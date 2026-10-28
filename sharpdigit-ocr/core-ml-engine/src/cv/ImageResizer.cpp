#include "../../include/cv/ImageResizer.h"

#include <opencv2/imgproc.hpp>

cv::Mat ImageResizer::resize(const cv::Mat &img, const cv::Size &target_size) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageResizer][resize]: Input image is empty.");
    }

    if (target_size.width <= 0 || target_size.height <= 0) {
        throw std::runtime_error("[ImageResizer][resize]: Invalid target size.");
    }

    cv::Mat resized;

    cv::resize(
            img,
            resized,
            target_size,
            0,
            0,
            cv::INTER_CUBIC);

    return resized;

}
