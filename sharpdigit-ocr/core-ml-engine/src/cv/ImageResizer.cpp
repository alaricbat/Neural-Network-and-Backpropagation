#include "../../include/cv/ImageResizer.h"

#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <vector>

cv::Mat ImageResizer::resize(
        const cv::Mat& img,
        const cv::Size& target_size
) const {

    if (img.empty()) {
        throw std::runtime_error(
                "[ImageResizer][resize]: Input image is empty."
        );
    }

    if (target_size.width <= 0 || target_size.height <= 0) {
        throw std::runtime_error(
                "[ImageResizer][resize]: Invalid target size."
        );
    }

    cv::Mat resized;

    std::vector<cv::Point> pts;
    cv::findNonZero(img, pts);

    if (!pts.empty()) {

        cv::Rect rect = cv::boundingRect(pts);

        int digit_w = rect.width;
        int digit_h = rect.height;

        int max_side = std::max(digit_h, digit_w);

        int pad = static_cast<int>(max_side * 0.2);
        int square_size = max_side + pad * 2;

        cv::Mat square_img = cv::Mat::zeros(
                square_size,
                square_size,
                CV_8UC1
        );

        int offset_y = pad + (max_side - digit_h) / 2;
        int offset_x = pad + (max_side - digit_w) / 2;

        cv::Mat digit_img = img(rect);

        cv::Rect dst_roi(
                offset_x,
                offset_y,
                digit_w,
                digit_h
        );

        digit_img.copyTo(square_img(dst_roi));

        cv::resize(
                square_img,
                resized,
                target_size,
                0,
                0,
                cv::INTER_AREA
        );

    } else {

        resized = cv::Mat::zeros(
                target_size,
                CV_8UC1
        );
    }

    cv::Mat final;

    cv::threshold(
            resized,
            final,
            0,
            255,
            cv::THRESH_BINARY | cv::THRESH_OTSU
    );

    return final;
}