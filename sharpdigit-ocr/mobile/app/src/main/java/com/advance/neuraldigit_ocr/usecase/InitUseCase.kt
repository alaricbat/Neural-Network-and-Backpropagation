package com.advance.neuraldigit_ocr.usecase

import com.advance.neuraldigit_ocr.repositories.InitRepository
import com.advance.neuraldigit_ocr.utils.Logger

class InitUseCase {

    private val TAG = "InitUseCase"

    private val _repo = InitRepository()

    operator fun invoke(
        onSuccess: () -> Unit,
        onFailure: (Exception) -> Unit
    ) {
        Logger.info(TAG, "[invoke]: execute.")
        _repo.init(
            onSuccess,
            onFailure
        )
    }

}