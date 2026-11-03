package com.advance.neuraldigit_ocr.usecase

import android.net.Uri
import com.advance.neuraldigit_ocr.repositories.RecognizeRepository
import com.advance.neuraldigit_ocr.utils.Logger

class RecognizeUseCase() {

    private val TAG = "RecognizeUseCase"

    private val _repo = RecognizeRepository()

    operator fun invoke(
        uri: Uri,
        onSuccess: (Int) -> Unit,
        onFailure: (Exception) -> Unit
    ) {
        Logger.info(TAG, "[invoke]: execute.")
        _repo.predictPhoto(
            uri,
            onSuccess,
            onFailure
        )
    }

}