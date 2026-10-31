package com.advance.neuraldigit_ocr.mvi

import com.advance.neuraldigit_ocr.state.CameraScreenState

data class CameraUiState(
    val cameraScreenState: CameraScreenState = CameraScreenState.IDLE
)

sealed interface CameraIntent {
    data object RequestCameraPermission : CameraIntent
    data object CapturedClicked : CameraIntent
    data object PredictClicked : CameraIntent
    data object CameraCancel : CameraIntent

    data object PredictBack : CameraIntent
}

sealed interface CameraEffect {
    data object RequestCameraPermission : CameraEffect
    data object StartCamera : CameraEffect
    data object StopCamera : CameraEffect
    data class ShowError(val msg: String) : CameraEffect
}