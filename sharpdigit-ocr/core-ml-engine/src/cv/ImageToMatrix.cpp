#include "../../include/cv/ImageToMatrix.h"

#include <opencv2/core.hpp>
#include <stdexcept>

Matrix ImageToMatrix::convert(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::invalid_argument(
                "[ImageToMatrix][convert]: Input image is empty."
        );
    }

    if (img.channels() != 1 || img.depth() != CV_8U) {
        throw std::invalid_argument(
                "[ImageToMatrix][convert]: Expected an 8-bit grayscale image."
        );
    }

    const int rows = img.rows;
    const int cols = img.cols;

    Matrix input(1, rows * cols, 0.0f);

    #pragma omp parallel for default(none) shared(img, input) firstprivate(rows, cols) schedule(static)
    for (int r = 0; r < rows; ++r) {
        const auto* pixels = img.ptr<unsigned char>(r);
        for (int c = 0; c < cols; ++c) {
            const int index = r * cols + c;
            input(0, index) = static_cast<float>(pixels[c]);
        }
    }

    return input;

}
