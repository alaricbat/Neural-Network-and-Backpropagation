#include "reader/TxtMatrixReader.h"

#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>

TxtMatrixReader::TxtMatrixReader(int expected_features)
: expected_features_(expected_features) {
    if (expected_features_ <= 0) {
        throw std::invalid_argument(
                "[TxtMatrixReader][constructor]: Expected feature count must be positive.");
    }
}

Matrix TxtMatrixReader::read(const std::string &file_path) const {

    std::ifstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error(
                "[TxtMatrixReader][read]: Cannot open TXT file: " + file_path);
    }

    int rows = 0;
    int cols = 0;

    if (!(file >> rows >> cols)) {
        throw std::runtime_error(
                "[TxtMatrixReader][read]: Invalid TXT header. Expected: rows cols.");
    }

    if (rows != 1 || cols != expected_features_) {
        throw std::runtime_error(
                "[TxtMatrixReader][read]: Unexpected matrix dimensions.");
    }

    Matrix input(rows, cols, 0.0f);

    for (int i = 0; i < rows * cols; ++i) {
        float value = 0.0f;

        if (!(file >> value)) {
            throw std::runtime_error(
                    "[TxtMatrixReader][read]: TXT file contains insufficient or invalid data."
            );
        }

        if (!std::isfinite(value)) {
            throw std::runtime_error(
                    "[TxtMatrixReader][read]: TXT file contains NaN or infinity."
            );
        }

        input.data[static_cast<std::size_t>(i)] = value;
    }

    // Reject unexpected extra numeric or non-whitespace content.
    file >> std::ws;

    if (file.peek() != std::char_traits<char>::eof()) {
        throw std::runtime_error(
                "[TxtMatrixReader][read]: TXT file contains unexpected trailing data."
        );
    }

    return input;
}