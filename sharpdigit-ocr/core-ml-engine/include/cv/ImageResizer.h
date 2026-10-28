#ifndef CORE_ML_ENGINE_IMAGERESIZER_H
#define CORE_ML_ENGINE_IMAGERESIZER_H

#include "interface/IImageResizer.h"

class ImageResizer final : public IImageResizer {
public:

    [[nodiscard]]
    cv::Mat resize(
            const cv::Mat &img,
            const cv::Size &target_size
    ) const override;
};

#endif //CORE_ML_ENGINE_IMAGERESIZER_H
