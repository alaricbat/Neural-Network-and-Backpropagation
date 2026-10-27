
#include "../include/MultilayerPerception.h"

#include <cmath>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>
#include <omp.h>

MultilayerPerceptron::MultilayerPerceptron(
        int feature_size,
        std::vector<int> hidden_size,
        int labels_num,
        int epochs,
        float lr,
        int batch_size
) :
feature_size(feature_size),
hidden_size(std::move(hidden_size)),
labels_num(labels_num),
epochs(epochs),
lr(lr),
batch_size(batch_size),
X_cache(0, 0),
relu(std::make_unique<ReLU>()),
softmax(std::make_shared<Softmax>()) {

    // 1. Validate configuration
    if (feature_size <= 0 ||
        labels_num <= 0 ||
        epochs <= 0 ||
        !std::isfinite(lr) ||
        lr <= 0.0f ||
        batch_size <= 0) {
        throw std::invalid_argument(
                "[MultilayerPerceptron][constructor]: Invalid MLP configuration."
        );
    }

    for (int size : this->hidden_size) {
        if (size <= 0) {
            throw std::invalid_argument(
                    "[MultilayerPerceptron][constructor]: Hidden layer size must be positive."
            );
        }
    }

    // 2. Build dimensions:
    // Input -> Hidden layers -> Output
    std::vector<int> dimensions;
    dimensions.reserve(this->hidden_size.size() + 2);

    dimensions.push_back(feature_size);

    for (int size : this->hidden_size) {
        dimensions.push_back(size);
    }

    dimensions.push_back(labels_num);

    // 3. Initialize weights and biases
    W.reserve(dimensions.size() - 1);
    b.reserve(dimensions.size() - 1);

    std::random_device rd;
    const std::uint32_t base_seed = rd();

    for (std::size_t i = 0; i + 1 < dimensions.size(); ++i) {

        const int fan_in = dimensions[i];
        const int fan_out = dimensions[i + 1];

        const bool is_output_layer =
                (i + 2 == dimensions.size());

        const float scale = std::sqrt(
                (is_output_layer ? 1.0f : 2.0f) /
                static_cast<float>(fan_in)
        );

        Matrix weights(fan_in, fan_out);

        const std::size_t count = weights.data.size();

        // Each thread gets its own RNG and distribution.
        #pragma omp parallel default(none) shared(base_seed, scale, i, weights, count)
        {
            const int thread_id = omp_get_thread_num();

            std::mt19937 gen(
                    base_seed +
                    static_cast<std::uint32_t>(thread_id) * 7919U +
                    static_cast<std::uint32_t>(i) * 104729U
            );

            std::normal_distribution<float> distribution(
                    0.0f, scale
            );

            #pragma omp for schedule(static)
            for (std::size_t j = 0; j < count; ++j) {
                weights.data[j] = distribution(gen);
            }
        }

        W.push_back(std::move(weights));
        b.emplace_back(1, fan_out, 0.0f);
    }
}

void MultilayerPerceptron::set_W(
        const std::vector<Matrix>& weights
) {
    if (weights.size() != W.size()) {
        throw std::invalid_argument(
                "[MultilayerPerceptron][set_W]: Weight layer count mismatch."
        );
    }

    for (std::size_t i = 0; i < weights.size(); ++i) {
        if (weights[i].rows != W[i].rows ||
            weights[i].cols != W[i].cols) {
            throw std::invalid_argument(
                    "[MultilayerPerceptron][set_W]: Weight matrix dimension mismatch."
            );
        }
    }

    W = weights;
}

void MultilayerPerceptron::set_b(
        const std::vector<Matrix>& biases
) {
    if (biases.size() != b.size()) {
        throw std::invalid_argument(
                "[MultilayerPerceptron][set_b]: Bias layer count mismatch."
        );
    }

    for (std::size_t i = 0; i < biases.size(); ++i) {
        if (biases[i].rows != b[i].rows ||
            biases[i].cols != b[i].cols) {
            throw std::invalid_argument(
                    "[MultilayerPerceptron][set_b]: Bias matrix dimension mismatch."
            );
        }
    }

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

    if (output.cols != labels_num || output.cols <= 0) {
        throw std::runtime_error(
                "[MultilayerPerceptron][predict]: Unexpected output matrix dimension."
        );
    }

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
