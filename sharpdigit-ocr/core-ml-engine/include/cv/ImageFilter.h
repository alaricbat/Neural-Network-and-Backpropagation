#ifndef CORE_ML_ENGINE_IMAGEFILTER_H
#define CORE_ML_ENGINE_IMAGEFILTER_H

#include "interface/IImageFilter.h"

class ImageFilter final : public IImageFilter {
public:
    [[nodiscard]]
    cv::Mat apply(const cv::Mat &img) const override;
};


#endif //CORE_ML_ENGINE_IMAGEFILTER_H
