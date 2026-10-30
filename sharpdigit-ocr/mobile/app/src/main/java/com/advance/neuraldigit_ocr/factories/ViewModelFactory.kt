package com.advance.neuraldigit_ocr.factories

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.advance.neuraldigit_ocr.viewmodel.OcrViewModel

class ViewModelFactory: ViewModelProvider.Factory {

    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(OcrViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return OcrViewModel() as T
        }

        throw IllegalArgumentException(
            "Unknown ViewModel class: ${modelClass.name}"
        )
    }

}