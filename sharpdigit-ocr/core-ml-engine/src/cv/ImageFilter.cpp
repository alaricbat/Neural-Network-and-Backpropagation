#include "../../include/cv/ImageFilter.h"

#include "opencv2/imgproc.hpp"

cv::Mat ImageFilter::apply(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][apply]: Input image is empty.");
    }

    cv::Mat gray;

    if (img.channels() == 1) {
        gray = img.clone();
    } else if (img.channels() == 3) {
        cv::cvtColor(img,gray,cv::COLOR_BGR2GRAY);
    } else if (img.channels() == 4) {
        cv::cvtColor(img, gray, cv::COLOR_BGRA2GRAY);
    } else {
        throw std::runtime_error("[ImageFilter][apply]: Unsupported image channel count.");
    }

    // 1. Gaussian Blur
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(11, 11), 0);

    // 2. Sharpen
    const cv::Mat sharpen_kernel = (
            cv::Mat_<float>(3, 3) <<
                    0.0f, -0.25f,  0.0f,
                    -0.25f,  2.0f, -0.25f,
                    0.0f, -0.25f,  0.0f
            );
    cv::Mat sharpened;
    cv::filter2D(
            blurred,
            sharpened,
            -1,
            sharpen_kernel);

    // 3. Automatic Canny threshold
    cv::Mat edges;
    cv::Canny(
            sharpened,
            edges,
            100,
            200);

    // 4. Dilate
    const cv::Mat kernel = cv::getStructuringElement(
            cv::MORPH_RECT,
            cv::Size(15, 15)
            );
    cv::Mat dilated;
    cv::dilate(
            edges,
            dilated,
            kernel
            );

    return dilated;
}
