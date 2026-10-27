
#include "../include/reader/BinaryMatrixReader.h"
#include "Matrix.h"

#include <fstream>
#include <limits>
#include <stdexcept>

Matrix BinaryMatrixReader::read(
        const std::filesystem::path& file_path
) {
    std::ifstream file(file_path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error(
                "[BinaryMatrixReader][read]: Cannot open file: " +
                file_path.string()
        );
    }

    std::int32_t rows = 0;
    std::int32_t cols = 0;

    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    if (!file || rows <= 0 || cols <= 0) {
        throw std::runtime_error(
                "[BinaryMatrixReader][read]: Invalid matrix dimensions."
        );
    }

    const auto count =
            static_cast<std::size_t>(rows) *
            static_cast<std::size_t>(cols);

    if (count > std::numeric_limits<std::size_t>::max() / sizeof(float)) {
        throw std::runtime_error(
                "[BinaryMatrixReader][read]: Matrix size overflow."
        );
    }

    Matrix matrix(rows, cols, 0.0f);

    file.read(
            reinterpret_cast<char*>(matrix.data.data()),
            static_cast<std::streamsize>(count * sizeof(float))
    );

    if (!file) {
        throw std::runtime_error(
                "[BinaryMatrixReader][read]: Insufficient matrix data."
        );
    }

    if (file.peek() != std::char_traits<char>::eof()) {
        throw std::runtime_error(
                "[BinaryMatrixReader][read]: Unexpected trailing data."
        );
    }

    return matrix;
}
