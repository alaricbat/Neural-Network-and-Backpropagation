package com.advance.neuraldigit_ocr.utils

import android.graphics.Bitmap

object NativeOcr {

    init {
        System.loadLibrary("nativelib")
    }

    external fun nativeInit(modelDir: String): Boolean

    external fun nativePredict(bitmap: Bitmap): Int

}