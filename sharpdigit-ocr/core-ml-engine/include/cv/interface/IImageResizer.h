#ifndef CORE_ML_ENGINE_IIMAGERESIZER_H
#define CORE_ML_ENGINE_IIMAGERESIZER_H

#include <opencv2/core.hpp>

class IImageResizer {
public:
    virtual ~IImageResizer() = default;
    [[nodiscard]]
    virtual cv::Mat resize(const cv::Mat& img,
                           const cv::Size& target_size) const = 0;
};

#endif //CORE_ML_ENGINE_IIMAGERESIZER_H
