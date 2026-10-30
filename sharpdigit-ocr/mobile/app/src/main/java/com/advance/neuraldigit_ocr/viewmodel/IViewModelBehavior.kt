package com.advance.neuraldigit_ocr.viewmodel

import com.advance.neuraldigit_ocr.mvi.CameraIntent
import com.advance.neuraldigit_ocr.state.CameraPermissionState

interface IViewModelBehavior {

    fun onIntent(intent: CameraIntent)

    fun onPermissionResult(cameraPermissionState: CameraPermissionState)

    fun onCameraStarted()

    fun onCameraClosed()

}