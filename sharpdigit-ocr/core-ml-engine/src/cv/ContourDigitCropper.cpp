#include "../../include/cv/ContourDigitCropper.h"

#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <vector>

cv::Mat ContourDigitCropper::crop(const cv::Mat &img) const {

    int img_h = img.rows;
    int img_w = img.cols;

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> valid_contours;

    cv::findContours(
            img,
            contours,
            hierarchy,
            cv::RETR_EXTERNAL,
            cv::CHAIN_APPROX_SIMPLE);

    for (const auto& c : contours) {
        cv::Rect rect = cv::boundingRect(c);
        double area = cv::contourArea(c);
        if (area < 50) {
            continue;
        }

        if (rect.width > img_w * 0.7 || rect.height > img_h * 0.7) {
            continue;
        }

        double aspect_ratio = (double) rect.width / rect.height;
        if (aspect_ratio < 0.1 || aspect_ratio > 3.0) {
            continue;
        }
        valid_contours.push_back(c);
    }

    const auto& candidates =
            valid_contours.empty() ? contours : valid_contours;

    if (candidates.empty()) {
        return img.clone();
    }

    const auto largest_it = std::max_element(
            candidates.begin(),
            candidates.end(),
            [](const auto& a, const auto& b) {
                return cv::contourArea(a) < cv::contourArea(b);
            }
    );

    cv::Rect rect = cv::boundingRect(*largest_it);
    cv::Mat digit_crop = img(rect);

    const int max_side = std::max(rect.width, rect.height);
    const int pad_size = std::max(
            max_side,
            static_cast<int>(max_side * 1.4)
    );

    cv::Mat digit_padded = cv::Mat::zeros(
            pad_size,
            pad_size,
            CV_8UC1
    );

    const int offset_x = (pad_size - rect.width) / 2;
    const int offset_y = (pad_size - rect.height) / 2;

    digit_crop.copyTo(
            digit_padded(
                    cv::Rect(
                            offset_x,
                            offset_y,
                            rect.width,
                            rect.height
                    )
            )
    );


    return digit_padded;

}