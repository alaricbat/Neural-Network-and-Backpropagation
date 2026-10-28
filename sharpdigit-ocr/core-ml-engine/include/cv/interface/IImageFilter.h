#ifndef CORE_ML_ENGINE_IIMAGEFILTER_H
#define CORE_ML_ENGINE_IIMAGEFILTER_H

#include "opencv2/core.hpp"

class IImageFilter {
public:
    virtual ~IImageFilter() = default;
    [[nodiscard]]
    virtual cv::Mat apply(const cv::Mat& img) const = 0;
};

#endif //CORE_ML_ENGINE_IIMAGEFILTER_H
