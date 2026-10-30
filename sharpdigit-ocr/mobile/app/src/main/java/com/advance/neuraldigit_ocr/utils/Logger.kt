package com.advance.neuraldigit_ocr.utils

import android.util.Log

object Logger {

    private const val DEFAULT_TAG = "DigitOCR"

    fun debug(tag: String = DEFAULT_TAG, message: String) {
        Log.d(tag, message)
    }

    fun info(tag: String = DEFAULT_TAG, message: String) {
        Log.i(tag, message)
    }

    fun warning(tag: String = DEFAULT_TAG, message: String) {
        Log.w(tag, message)
    }

    fun error(
        tag: String = DEFAULT_TAG,
        message: String,
        throwable: Throwable? = null
    ) {
        Log.e(tag, message, throwable)
    }

    fun verbose(tag: String = DEFAULT_TAG, message: String) {
        Log.v(tag, message)
    }

}