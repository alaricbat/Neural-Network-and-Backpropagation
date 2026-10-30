package com.advance.neuraldigit_ocr.viewmodel

import com.advance.neuraldigit_ocr.mvi.CameraIntent
import com.advance.neuraldigit_ocr.state.CameraScreenState

interface IViewModelBehavior {

    fun onIntent(intent: CameraIntent)

    fun onStateChanged(state: CameraScreenState)

    fun updateCapturedImage(path: String)

    fun capturePhoto()

    fun initModel()

}