#ifndef CORE_ML_ENGINE_IDIGITCROPPER_H
#define CORE_ML_ENGINE_IDIGITCROPPER_H

#include <opencv2/core.hpp>

class IDigitCropper {

public:
    virtual ~IDigitCropper() = default;
    [[nodiscard]]
    virtual cv::Mat crop(const cv::Mat& img) const = 0;
};

#endif //CORE_ML_ENGINE_IDIGITCROPPER_H
