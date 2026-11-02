#ifndef CORE_ML_ENGINE_IIMAGEFILTER_H
#define CORE_ML_ENGINE_IIMAGEFILTER_H

#include "opencv2/core.hpp"

class IImageFilter {
public:
    virtual ~IImageFilter() = default;
    [[nodiscard]]
    virtual cv::Mat apply(const cv::Mat& img) const = 0;
    [[nodiscard]]
    virtual cv::Mat gray(const cv::Mat& img) const = 0;
    [[nodiscard]]
    virtual cv::Mat blur(const cv::Mat& img) const = 0;
    [[nodiscard]]
    virtual cv::Mat binary(const cv::Mat& img) const = 0;
    [[nodiscard]]
    virtual cv::Mat crop(const cv::Mat& img) const = 0;
    [[nodiscard]]
    virtual cv::Mat sharp(const cv::Mat& img) const = 0;
    [[nodiscard]]
    virtual cv::Mat findObjectCenter(const cv::Mat& img) const = 0;
    [[nodiscard]]
    virtual cv::Mat dilate(const cv::Mat &img) const = 0;
};

#endif //CORE_ML_ENGINE_IIMAGEFILTER_H
