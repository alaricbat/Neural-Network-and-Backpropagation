#include <complex>
#include "ml/Softmax.h"

Softmax::Softmax() : lastOut(0, 0) {}

Matrix Softmax::forward(const Matrix& Z) {
    Matrix out(Z.rows, Z.cols);

    #pragma omp parallel for default(none) shared(Z, out)
    for (int i = 0; i < Z.rows; ++i) {
        float max_val = Z(i, 0);
        for (int j = 1; j < Z.cols; ++j) {
            max_val = std::max(max_val, Z(i, j));
        }

        float sum = 0.0f;
        for (int j = 0; j < Z.cols; ++j) {
            out(i, j) = std::exp(Z(i, j) - max_val);
            sum += out(i, j);
        }

        for (int j = 0; j < Z.cols; ++j) {
            out(i, j) /= sum;
        }
    }
    lastOut = out;
    return out;
}

Matrix Softmax::backward(const Matrix& y_one_hot) {
    Matrix dZ(lastOut.rows, lastOut.cols);
    int total = static_cast<int>(lastOut.data.size());

    #pragma omp parallel for default(none) shared(total, dZ, y_one_hot)
    for (int i = 0; i < total; ++i) {
        dZ.data[i] = lastOut.data[i] - y_one_hot.data[i];
    }
    return dZ;
}