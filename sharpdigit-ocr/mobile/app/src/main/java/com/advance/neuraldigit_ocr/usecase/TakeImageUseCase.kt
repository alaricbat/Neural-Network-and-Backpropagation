package com.advance.neuraldigit_ocr.usecase

import androidx.camera.core.ImageCapture
import com.advance.neuraldigit_ocr.repositories.CameraRepository
import com.advance.neuraldigit_ocr.utils.Logger
import java.io.File

class TakeImageUseCase() {

    private val TAG = "TakeImageUseCase"

    private val _repo = CameraRepository()

    operator fun invoke(
        imageCapture: ImageCapture,
        onSuccess: (File) -> Unit,
        onFailure: (Exception) -> Unit
    ) {
        Logger.info(TAG, "[invoke]: execute.")
        _repo.takePicture(
            imageCapture,
            onSuccess,
            onFailure
        )
    }

}