#include "../include/Matrix.h"

Matrix::Matrix(int r, int c, float initial_value):
    rows(r), cols(c), data(r * c, initial_value){}

float &Matrix::operator()(int r, int c) {
    return data[r * cols + c];
}

const float &Matrix::operator()(int r, int c) const {
    return data[r * cols + c];
}

Matrix Matrix::matmul(const Matrix &A, const Matrix &B) {
    if (A.cols != B.cols) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }
    Matrix C(A.rows, B.rows, 0.0f);
    #pragma omp parallel for default(none) shared(A, B, C) collapse(2) schedule(static)
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < A.cols; ++k) {
                sum += A(i, k) * B(k, j);
            }
            C(i, j) = sum;
        }
    }
    return C;
}

Matrix Matrix::transpose() const {
    Matrix T(cols, rows, 0.0f);
    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; ++j) {
            T(j, i) = (*this)(i, j);
        }
    }
    return T;
}

void Matrix::addBiasRow(const Matrix& bias) {
    if (cols != bias.cols || bias.rows != 1) {
        throw std::invalid_argument("Invalid bias dimensions.");
    }
    #pragma omp parallel for default(none) shared(bias) collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            (*this)(i, j) += bias(0, j);
        }
    }
}

