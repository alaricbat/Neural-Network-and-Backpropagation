#ifndef CORE_ML_ENGINE_IIMAGETOMATRIX_H
#define CORE_ML_ENGINE_IIMAGETOMATRIX_H

#include "../../ml/Matrix.h"
#include "opencv2/core.hpp"

class IImageToMatrix {

public:
    virtual ~IImageToMatrix() = default;
    [[nodiscard]]
    virtual Matrix convert(const cv::Mat& img) const = 0;
};

#endif //CORE_ML_ENGINE_IIMAGETOMATRIX_H
