#ifndef CORE_ML_ENGINE_IMAGEPROCESSOR_H
#define CORE_ML_ENGINE_IMAGEPROCESSOR_H

#include "../../../../../../../../core-ml-engine/include/cv/interface/IImageFilter.h"
#include "../../../../../../../../core-ml-engine/include/cv/interface/IDigitCropper.h"
#include "../../../../../../../../core-ml-engine/include/cv/interface/IImageResizer.h"
#include "../../../../../../../../core-ml-engine/include/cv/interface/IImageToMatrix.h"

#include "../../../../../../../../core-ml-engine/include/ml/Matrix.h"

#include <memory>
#include <opencv2/imgproc.hpp>

class ImageProcessor {
public:

    ImageProcessor();

    ImageProcessor(
            std::unique_ptr<IImageFilter> filter,
            std::unique_ptr<IImageResizer> resizer,
            std::unique_ptr<IImageToMatrix> converter
    );

    [[nodiscard]]
    Matrix process(const cv::Mat& img) const;

private:
    std::unique_ptr<IImageFilter> filter_;
    std::unique_ptr<IImageResizer> resizer_;
    std::unique_ptr<IImageToMatrix> converter_;
};

#endif //CORE_ML_ENGINE_IMAGEPROCESSOR_H
