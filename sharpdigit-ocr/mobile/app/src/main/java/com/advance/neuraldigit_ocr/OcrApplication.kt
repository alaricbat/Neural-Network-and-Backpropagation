package com.advance.neuraldigit_ocr

import android.app.Application
import com.advance.neuraldigit_ocr.utils.Context
import com.advance.neuraldigit_ocr.utils.Logger

private const val TAG = "OcrApplication"

class OcrApplication: Application() {

    override fun onCreate() {
        super.onCreate()
        Logger.info(TAG, "[onCreate]: execute.")
        Context.init(this)
    }

}