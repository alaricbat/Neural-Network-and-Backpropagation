
#ifndef CORE_ML_ENGINE_MULTILAYERPERCEPTION_H
#define CORE_ML_ENGINE_MULTILAYERPERCEPTION_H

#include <cstddef>
#include <memory>
#include <random>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "Matrix.h"
#include "ReLU.h"
#include "Softmax.h"
#include "interface/IActivation.h"

class MultilayerPerceptron {
private:
    int feature_size;
    std::vector<int> hidden_size;
    int labels_num;
    int epochs;
    float lr;
    int batch_size;

    std::vector<Matrix> W;
    std::vector<Matrix> b;

    std::vector<Matrix> Z_cache;
    std::vector<Matrix> A_cache;
    Matrix X_cache;

    std::unique_ptr<IActivation> relu;
    std::shared_ptr<Softmax> softmax;

public:
    explicit MultilayerPerceptron(
            int feature_size = 784,
            std::vector<int> hidden_size = {128},
            int labels_num = 10,
            int epochs = 100,
            float lr = 0.001f,
            int batch_size = 64
    );

    void set_W(const std::vector<Matrix>& weights);
    void set_b(const std::vector<Matrix>& biases);

    [[nodiscard]] Matrix forward(const Matrix& X);
    [[nodiscard]] int predict(const Matrix& X);
};

#endif // CORE_ML_ENGINE_MULTILAYERPERCEPTION_H
