package com.advance.neuraldigit_ocr.utils

import android.graphics.Bitmap

object NativeOcr {

    init {
        System.loadLibrary("sharpdigit_ocr")
    }

    external fun nativeInit(modelDir: String): Boolean

    external fun nativePredict(bitmap: Bitmap): Int

}