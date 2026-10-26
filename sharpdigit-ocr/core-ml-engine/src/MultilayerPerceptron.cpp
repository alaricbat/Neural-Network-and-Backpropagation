
#include "../include/MultilayerPerception.h"

#include <utility>

MultilayerPerceptron::MultilayerPerceptron(
        int feature_size,
        std::vector<int> hidden_size,
        int labels_num,
        int epochs,
        float lr,
        int batch_size
)
        : feature_size(feature_size),
          hidden_size(std::move(hidden_size)),
          labels_num(labels_num),
          epochs(epochs),
          lr(lr),
          batch_size(batch_size),
          X_cache(0, 0),
          relu(std::make_unique<ReLU>()),
          softmax(std::make_shared<Softmax>()) {

    if (feature_size <= 0 ||
        labels_num <= 0 ||
        epochs <= 0 ||
        lr <= 0.0f ||
        batch_size <= 0) {
        throw std::invalid_argument(
                "Invalid MLP configuration."
        );
    }

    for (int i : hidden_size) {
        if (i <= 0) {
            throw std::invalid_argument(
                    "Hidden layer size must be positive."
            );
        }
    }

    // Build network dimensions:
    // Input -> Hidden layers -> Output
    std::vector<int> num_layers;
    num_layers.reserve(this->hidden_size.size() + 2);

    num_layers.push_back(feature_size);

    for (int i : hidden_size) {
        num_layers.push_back(i);
    }

    num_layers.push_back(labels_num);

    std::random_device rd;
    std::mt19937 gen(rd());

    for (std::size_t i = 0;
         i + 1 < num_layers.size();
         ++i) {

        const int fan_in = num_layers[i];
        const int fan_out = num_layers[i + 1];

        // He initialization for hidden layers.
        // A smaller Xavier-style scale for the output layer.
        const bool is_output_layer =
                (i + 2 == num_layers.size());

        const float scale = std::sqrt(
                (is_output_layer ? 1.0f : 2.0f) /
                static_cast<float>(fan_in)
        );

        std::normal_distribution<float> dist(0.0f, scale);

        Matrix w(fan_in, fan_out);

        const std::size_t total_size =
                static_cast<std::size_t>(w.rows) *
                static_cast<std::size_t>(w.cols);

        float* ptr = w.data.data();

        for (std::size_t idx = 0; idx < w.data.size(); ++idx) {
            ptr[idx] = dist(gen);
        }

        W.push_back(std::move(w));

        // Bias shape: 1 x fan_out
        Matrix bias(1, fan_out, 0.0f);
        b.push_back(std::move(bias));
    }
}

void MultilayerPerceptron::set_W(
        const std::vector<Matrix>& weights) {

    if (weights.size() != W.size()) {
        throw std::invalid_argument(
                "Weight layer count mismatch."
        );
    }

    for (std::size_t i = 0; i < weights.size(); ++i) {
        if (weights[i].rows != W[i].rows ||
            weights[i].cols != W[i].cols) {
            throw std::invalid_argument(
                    "Weight matrix dimension mismatch."
            );
        }
    }

    for (std::size_t i = 0; i < weights.size(); ++i) {
        W[i] = weights[i];
    }
}

void MultilayerPerceptron::set_b(
        const std::vector<Matrix>& biases) {

    if (biases.size() != b.size()) {
        throw std::invalid_argument(
                "Bias layer count mismatch."
        );
    }

    for (std::size_t i = 0; i < biases.size(); ++i) {
        if (biases[i].rows != b[i].rows ||
            biases[i].cols != b[i].cols) {
            throw std::invalid_argument(
                    "Bias matrix dimension mismatch."
            );
        }
    }

    for (std::size_t i = 0; i < biases.size(); ++i) {
        b[i] = biases[i];
    }
}


Matrix MultilayerPerceptron::forward(const Matrix& X) {
    if (X.rows <= 0 || X.cols != feature_size) {
        throw std::invalid_argument(
                "Input matrix dimension mismatch."
        );
    }

    if (W.empty()|| W.size() != b.size()) {
        throw std::runtime_error(
                "Invalid network weights or biases."
        );
    }

    X_cache = X;

    Z_cache.clear();
    A_cache.clear();

    A_cache.push_back(X);

    Matrix current_input = X;

    const std::size_t num_layers = W.size();

    for (std::size_t i = 0; i < num_layers; ++i) {
        Matrix z = Matrix::matmul(current_input, W[i]);
        z.addBiasRow(b[i]);

        Z_cache.push_back(z);

        Matrix out = (i == num_layers - 1)
                     ? softmax->forward(z)
                     : relu->forward(z);

        A_cache.push_back(out);

        current_input = out;
    }

    return current_input;
}


int MultilayerPerceptron::predict(const Matrix& X) {
    if (X.rows != 1) {
        throw std::invalid_argument(
                "predict() expects exactly one sample."
        );
    }

    Matrix output = forward(X);

    if (output.cols != labels_num || output.cols <= 0) {
        throw std::runtime_error(
                "Unexpected output matrix dimension."
        );
    }

    int best_class = 0;
    float max_val = output(0, 0);

    for (int j = 1; j < output.cols; ++j) {
        if (output(0, j) > max_val) {
            max_val = output(0, j);
            best_class = j;
        }
    }

    return best_class;
}
