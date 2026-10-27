
#include "../include/MultilayerPerception.h"

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

MultilayerPerceptron::MultilayerPerceptron(
        int feature_size) :
        feature_size(feature_size),
        X_cache(0, 0),
        relu(std::make_unique<ReLU>()),
        softmax(std::make_shared<Softmax>()) {}

void MultilayerPerceptron::set_W(
        const std::vector<Matrix>& weights
) {
    W = weights;
}

void MultilayerPerceptron::set_b(
        const std::vector<Matrix>& biases
) {
    b = biases;
}

Matrix MultilayerPerceptron::forward(const Matrix& X) {
    if (X.rows <= 0 || X.cols != feature_size) {
        throw std::invalid_argument(
                "[MultilayerPerceptron][forward]: Input matrix dimension mismatch."
        );
    }

    if (W.empty() || W.size() != b.size()) {
        throw std::runtime_error(
                "[MultilayerPerceptron][forward]: Invalid network weights or biases."
        );
    }

    X_cache = X;

    Z_cache.clear();
    A_cache.clear();

    const std::size_t num_layers = W.size();

    Z_cache.reserve(num_layers);
    A_cache.reserve(num_layers + 1);

    A_cache.push_back(X);

    for (std::size_t i = 0; i < num_layers; ++i) {
        const Matrix& layer_input = A_cache.back();

        // Matrix::matmul can be parallelized internally.
        Matrix z = Matrix::matmul(layer_input, W[i]);

        z.addBiasRow(b[i]);
        Z_cache.push_back(std::move(z));

        const Matrix& current_z = Z_cache.back();

        Matrix output =
                (i + 1 == num_layers)
                ? softmax->forward(current_z)
                : relu->forward(current_z);

        A_cache.push_back(std::move(output));
    }

    return A_cache.back();
}

int MultilayerPerceptron::predict(const Matrix& X) {
    if (X.rows != 1) {
        throw std::invalid_argument(
                "[MultilayerPerceptron][predict]: expects exactly one sample."
        );
    }

    Matrix output = forward(X);

    int best_class = 0;
    float max_value = output(0, 0);

    for (int j = 1; j < output.cols; ++j) {
        const float value = output(0, j);

        if (value > max_value) {
            max_value = value;
            best_class = j;
        }
    }

    return best_class;
}
