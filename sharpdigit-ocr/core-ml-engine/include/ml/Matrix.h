#ifndef CORE_ML_ENGINE_MATRIX_H
#define CORE_ML_ENGINE_MATRIX_H

#include <vector>
#include <stdexcept>
#include <omp.h>

class Matrix {

public:
    int rows;
    int cols;
    std::vector<float> data;

    explicit Matrix(int r = 0, int c = 0, float initial_value = 0.0f);
    float& operator()(int r, int c);
    const float& operator()(int r, int c) const;

    [[nodiscard]] static Matrix dot(const Matrix& A, const Matrix& B);
    [[nodiscard]] Matrix transpose() const;

    void addBiasRow(const Matrix& bias);

};

#endif //CORE_ML_ENGINE_MATRIX_H
