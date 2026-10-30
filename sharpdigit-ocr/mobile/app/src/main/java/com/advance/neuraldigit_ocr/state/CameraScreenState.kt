package com.advance.neuraldigit_ocr.state

enum class CameraScreenState {
    IDLE,
    CAMERA_GRANTED,
    CAMERA_PREVIEW,
    CAMERA_PREVIEW_CANCEL,
    CAPTURE_PROCESSING,
    CAPTURE_FINISH,

    IMAGE_PREVIEW,
    OCR_PROCESSING,
    OCR_FINISH,
    ERROR
}