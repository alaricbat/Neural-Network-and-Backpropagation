#include <jni.h>
#include <android/bitmap.h>

#include <opencv2/imgproc.hpp>

#include "ml/MultilayerPerception.h"
#include "ml/weight/ModelWeightReader.h"
#include "cv/ImageProcessor.h"

#include <filesystem>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

    std::mutex g_mutex;
    std::unique_ptr<MultilayerPerceptron> g_model;
    std::unique_ptr<ImageProcessor> g_processor;

    void throwJavaException(JNIEnv* env, const char* message) {
        jclass exception_class =
                env->FindClass("java/lang/IllegalStateException");

        if (exception_class != nullptr) {
            env->ThrowNew(exception_class, message);
        }
    }

    cv::Mat bitmapToBGR(JNIEnv* env, jobject bitmap) {
        if (bitmap == nullptr) {
            throw std::invalid_argument("Bitmap cannot be null.");
        }

        AndroidBitmapInfo  info{};

        if (AndroidBitmap_getInfo(env, bitmap, &info)
            != ANDROID_BITMAP_RESULT_SUCCESS) {
            throw std::runtime_error("Cannot get Bitmap information.");
        }

        if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            throw std::runtime_error("Expected an RGBA_8888 Bitmap.");
        }

        void* pixels = nullptr;

        if (AndroidBitmap_lockPixels(env, bitmap, &pixels)
            != ANDROID_BITMAP_RESULT_SUCCESS) {
            throw std::runtime_error("Cannot lock Bitmap pixels.");
        }

        cv::Mat bgr;

        try {
            cv::Mat rgba(
                    static_cast<int>(info.height),
                    static_cast<int>(info.width),
                    CV_8UC4,
                    pixels,
                    info.stride
            );
            cv::cvtColor(rgba, bgr, cv::COLOR_RGBA2BGR);
            AndroidBitmap_unlockPixels(env, bitmap);

            return bgr;
        } catch (...) {
            AndroidBitmap_unlockPixels(env, bitmap);
            throw;
        }
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_advance_neuraldigit_ocr_NativeOcr_nativeInit(
        JNIEnv* env,
        jobject,
        jstring modelDirectory
) {

    if (modelDirectory == nullptr) {
        throwJavaException(env, "Model directory cannot be null.");
        return JNI_FALSE;
    }

    const char* raw_path =
            env->GetStringUTFChars(modelDirectory, nullptr);

    if (raw_path == nullptr) {
        return JNI_FALSE;
    }

    const std::string model_path(raw_path);
    env->ReleaseStringUTFChars(modelDirectory, raw_path);

    try {

        auto model = std::make_unique<MultilayerPerceptron>();

        const auto weights = ModelWeightReader::read(
                std::filesystem::path(model_path)
        );

        model->set_W(weights.weights);
        model->set_b(weights.biases);

        auto processor = std::make_unique<ImageProcessor>();

        std::lock_guard<std::mutex> lock(g_mutex);

        g_model = std::move(model);
        g_processor = std::move(processor);

        return JNI_TRUE;

    } catch (const std::exception& e) {
        throwJavaException(env, e.what());
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_advance_neuraldigit_ocr_NativeOcr_nativePredict(
        JNIEnv* env,
        jobject /* thiz */,
        jobject bitmap
) {
    try {
        std::lock_guard<std::mutex> lock(g_mutex);

        if (!g_model || !g_processor) {
            throw std::runtime_error(
                    "OCR engine has not been initialized."
            );
        }

        cv::Mat image = bitmapToBGR(env, bitmap);

        Matrix input = g_processor->process(image);

        return static_cast<jint>(g_model->predict(input));

    } catch (const std::exception& e) {
        throwJavaException(env, e.what());
        return -1;
    }
}