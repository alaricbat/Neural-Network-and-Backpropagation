package com.advance.neuraldigit_ocr.data

import android.net.Uri
import androidx.camera.core.ImageCapture

data class OcrData (
    val imageCapture: ImageCapture? = null,
    val capturedImagePath: Uri? = null,
    val predictedValue: Int = 0
)