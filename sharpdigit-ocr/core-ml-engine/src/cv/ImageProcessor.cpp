#include "../../include/cv/ImageFilter.h"
#include "../../include/cv/ImageResizer.h"
#include "../../include/cv/ImageToMatrix.h"
#include "../../include/cv/ImageProcessor.h"
#include "../../include/cv/ContourDigitCropper.h"

#include <stdexcept>
#include <utility>

ImageProcessor::ImageProcessor() : ImageProcessor(
        std::make_unique<ImageFilter>(),
        std::make_unique<ContourDigitCropper>(),
        std::make_unique<ImageResizer>(),
        std::make_unique<ImageToMatrix>()
        ){}

ImageProcessor::ImageProcessor(
        std::unique_ptr<IImageFilter> filter,
        std::unique_ptr<IDigitCropper> cropper,
        std::unique_ptr<IImageResizer> resizer,
        std::unique_ptr<IImageToMatrix> converter
)
        : filter_(std::move(filter)),
          cropper_(std::move(cropper)),
          resizer_(std::move(resizer)),
          converter_(std::move(converter)) {}

Matrix ImageProcessor::process(const cv::Mat& img) const {
    if (img.empty()) {
        throw std::invalid_argument("[ImageProcessor][process]: Input image cannot be empty.");
    }

    cv::Mat filtered = filter_->apply(img);
    cv::Mat cropped = cropper_->crop(filtered);
    cv::Mat resized = resizer_->resize(cropped, cv::Size(28, 28));

    return converter_->convert(resized);
}