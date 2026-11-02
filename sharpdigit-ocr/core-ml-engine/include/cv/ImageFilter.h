#ifndef CORE_ML_ENGINE_IMAGEFILTER_H
#define CORE_ML_ENGINE_IMAGEFILTER_H

#include "interface/IImageFilter.h"

class ImageFilter final : public IImageFilter {
public:
    [[nodiscard]]
    cv::Mat apply(const cv::Mat &img) const override;
    [[nodiscard]]
    cv::Mat gray(const cv::Mat &img) const override;
    [[nodiscard]]
    cv::Mat blur(const cv::Mat &img) const override;
    [[nodiscard]]
    cv::Mat binary(const cv::Mat &img) const override;
    [[nodiscard]]
    cv::Mat crop(const cv::Mat &img) const override;
    [[nodiscard]]
    cv::Mat sharp(const cv::Mat &img) const override;
    [[nodiscard]]
    cv::Mat findObjectCenter(const cv::Mat &img) const override;
    [[nodiscard]]
    cv::Mat dilate(const cv::Mat &img) const override;
};


#endif //CORE_ML_ENGINE_IMAGEFILTER_H
