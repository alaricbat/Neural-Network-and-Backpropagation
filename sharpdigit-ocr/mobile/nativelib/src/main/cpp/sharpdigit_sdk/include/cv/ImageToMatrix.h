#ifndef CORE_ML_ENGINE_IMAGETOMATRIX_H
#define CORE_ML_ENGINE_IMAGETOMATRIX_H

#include "../../../../../../../../core-ml-engine/include/cv/interface/IImageToMatrix.h"

class ImageToMatrix final : public IImageToMatrix {
public:
    [[nodiscard]]
    Matrix convert(const cv::Mat &img) const override;
};

#endif //CORE_ML_ENGINE_IMAGETOMATRIX_H
