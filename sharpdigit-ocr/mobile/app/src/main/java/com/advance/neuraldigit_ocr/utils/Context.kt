package com.advance.neuraldigit_ocr.utils

import android.annotation.SuppressLint
import android.content.Context

@SuppressLint("StaticFieldLeak")
object Context {

    private lateinit var _context: Context

    fun init(context: Context) {
        _context = context
    }

    fun get() : Context {
        return _context
    }

}