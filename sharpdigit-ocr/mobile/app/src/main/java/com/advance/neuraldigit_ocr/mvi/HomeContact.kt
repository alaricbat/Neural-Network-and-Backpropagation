package com.advance.neuraldigit_ocr.mvi

import com.advance.neuraldigit_ocr.state.CameraPermissionState

data class HomeUiState(
    val cameraPermission: CameraPermissionState = CameraPermissionState.UNKNOWN,
    val isCameraReady: Boolean = false,
    val isCapturing: Boolean = false,
    val isRecognizing: Boolean = false,
    val capturedImagePath: String? = null,
    val recognizedText: String? = null,
    val errorMessage: String? = null
)

sealed interface CameraIntent {
    data object RequestCameraPermission: CameraIntent
    data object CapturedClick : CameraIntent
    data object RetryClicked: CameraIntent
}

sealed interface CameraEffect {
    data object RequestCameraPermission: CameraEffect
    data object StartCamera: CameraEffect
    data class ShowError(val msg: String): CameraEffect
}