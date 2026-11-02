#ifndef CORE_ML_ENGINE_CONTOURDIGITCROPPER_H
#define CORE_ML_ENGINE_CONTOURDIGITCROPPER_H

#include "interface/IDigitCropper.h"

class ContourDigitCropper final : public IDigitCropper {
public:
    [[nodiscard]]
    cv::Mat crop(const cv::Mat &img) const override;
};

#endif //CORE_ML_ENGINE_CONTOURDIGITCROPPER_H
