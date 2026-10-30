package com.advance.neuraldigit_ocr.repositories.base

import androidx.camera.core.ImageCapture
import androidx.camera.core.ImageCaptureException
import java.io.File

interface ICameraRepository {

    fun takePicture(
        imageCapture: ImageCapture,
        onImgSaved: (File) -> Unit,
        onError: (ImageCaptureException) -> Unit
    )

}