#include "../include/ReLU.h"

ReLU::ReLU(): lastZ(0, 0) {}

Matrix ReLU::forward(const Matrix &Z) {
    lastZ = Z;
    Matrix out(Z.rows, Z.cols);
    int total = static_cast<int>(Z.data.size());

    #pragma omp parallel for default(none) shared(total, out, Z)
    for (int i = 0; i < total; ++i) {
        out.data[i] = std::max(0.0f, Z.data[i]);
    }
    return out;
}

Matrix ReLU::backward(const Matrix &dA) {
    Matrix dZ(dA.rows, dA.cols);
    int total = static_cast<int>(dA.data.size());

    #pragma omp parallel for default(none) shared(total, dZ, dA)
    for (int i = 0; i < total; ++i) {
        dZ.data[i] = (lastZ.data[i] > 0) ? dA.data[i] : 0.0f;
    }
    return dZ;
}