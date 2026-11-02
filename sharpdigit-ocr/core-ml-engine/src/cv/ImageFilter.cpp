#include "../../include/cv/ImageFilter.h"

#include "opencv2/imgproc.hpp"

struct Contour {
    int start_x;
    int end_x;
    int start_y;
    int end_y;
};

cv::Mat ImageFilter::gray(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][gray]: Input image is empty.");
    }

    cv::Mat gray;

    if (img.channels() == 1) {
        gray = img.clone();
    } else if (img.channels() == 3) {
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    } else if (img.channels() == 4) {
        cv::cvtColor(img, gray, cv::COLOR_BGRA2GRAY);
    } else {
        throw std::runtime_error("[ImageFilter][gray]: Unsupported image channel count.");
    }

    return gray;
}

cv::Mat ImageFilter::blur(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][blur]: Input image is empty.");
    }

    cv::Mat blurred;
    cv::GaussianBlur(img, blurred, cv::Size(21, 21), 0);

    return blurred;
}

cv::Mat ImageFilter::binary(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][binary]: Input image is empty.");
    }

    cv::Mat binary_img;
    double thresh = 0;
    double maxval = 255;
    cv::threshold(img, binary_img, thresh, maxval, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    return binary_img;
}

cv::Mat ImageFilter::crop(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][crop]: Input image is empty.");
    }

    int h = img.rows;
    int w = img.cols;

    int pad = static_cast<int>(std::max(w / 2, h / 2) * 0.4);

    int start_y = pad;
    int end_y = std::min(h, h - pad);
    int start_x = pad;
    int end_x = std::min(w, w - pad);

    int cropped_x = std::abs(end_x - start_x);
    int cropped_y = std::abs(end_y - start_y);

    cv::Mat cropped = cv::Mat::zeros(
            cv::Size(
                    cropped_x,
                    cropped_y
            ),
            CV_8UC1);

    if (cropped_y > 0 && cropped_x > 0) {
        cv::Rect roi(start_x, start_y, cropped_x, cropped_y);
        img(roi).copyTo(cropped);
    }

    return cropped;
}

cv::Mat ImageFilter::sharp(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][sharp]: Input image is empty.");
    }

    const cv::Mat sharpen_kernel = (
            cv::Mat_<float>(3, 3) <<
                                  0.0f, -0.25f,  0.0f,
                    -0.25f,  2.0f, -0.25f,
                    0.0f, -0.25f,  0.0f
    );
    cv::Mat sharpened;
    cv::filter2D(
            img,
            sharpened,
            -1,
            sharpen_kernel);

    return sharpened;
}

cv::Mat ImageFilter::findObjectCenter(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][findObjectCenter]: Input image is empty.");
    }

    float pad_ratio = 0.25f;

    int img_h = img.rows;
    int img_w = img.cols;

    int range_x_start = static_cast<int>(static_cast<float>(img_w) * pad_ratio);
    int range_x_end = img_w - range_x_start;
    int range_y_start = static_cast<int>(static_cast<float>(img_h) * pad_ratio);
    int range_y_end = img_h - range_y_start;

    std::vector<std::vector<cv::Point>> external_contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img, external_contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    std::vector<Contour> valid_contours;
    for (const auto & external_contour : external_contours) {

        cv::Rect rect = cv::boundingRect(external_contour);
        int x = rect.x;
        int y = rect.y;
        int w = rect.width;
        int h = rect.height;

        double area = cv::contourArea(external_contour);

        int x_0 = x;
        int x_1 = x + w;
        int y_0 = y;
        int y_1 = y + h;

        bool is_in_center = false;

        if ((range_x_start <= x_0 && x_0 <= range_x_end) &&
            (range_x_start <= x_1 && x_1 <= range_x_end) &&
            (range_y_start <= y_0 && y_0 <= range_y_end) &&
            (range_y_start <= y_1 && y_1 <= range_y_end)) {
            is_in_center = true;
        }

        if (area > 1000 && is_in_center) {
            Contour contours{};
            contours.start_x = x_0;
            contours.end_x = x_1;
            contours.start_y = y_0;
            contours.end_y = y_1;
            valid_contours.push_back(contours);
        }
    }

    cv::Mat filtered_img = cv::Mat::zeros(
            cv::Size(img_w, img_h),
            CV_8UC1);

    for(const auto & c : valid_contours) {
        int x = c.start_x;
        int y = c.start_y;
        int w = c.end_x - c.start_x;
        int h = c.end_y - c.start_y;

        if (w > 0 && h > 0) {
            cv::Rect roi(x, y, w, h);
            img(roi).copyTo(filtered_img(roi));
        }
    }

    return filtered_img;
}

cv::Mat ImageFilter::dilate(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][dilate]: Input image is empty.");
    }

    const cv::Mat kernel = cv::getStructuringElement(
            cv::MORPH_RECT,
            cv::Size(13, 13)
    );
    cv::Mat dilated;
    cv::dilate(
            img,
            dilated,
            kernel
    );

    return dilated;
}

cv::Mat ImageFilter::apply(const cv::Mat &img) const {

    if (img.empty()) {
        throw std::runtime_error("[ImageFilter][apply]: Input image is empty.");
    }

    cv::Mat gray = this->gray(img);
    cv::Mat blurred = this->blur(gray);
    cv::Mat binary = this->binary(blurred);
    cv::Mat cropped = this->crop(binary);
    cv::Mat sharpened = this->sharp(cropped);
    cv::Mat center_obj = this->findObjectCenter(sharpened);
    cv::Mat dilated = this->dilate(center_obj);

    return dilated;
}