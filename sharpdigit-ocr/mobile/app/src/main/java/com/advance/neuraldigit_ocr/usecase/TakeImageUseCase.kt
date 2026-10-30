package com.advance.neuraldigit_ocr.usecase

import androidx.camera.core.ImageCapture
import com.advance.neuraldigit_ocr.repositories.CameraRepository
import java.io.File

class TakeImageUseCase() {

    private val _repo = CameraRepository()

    operator fun invoke(
        imageCapture: ImageCapture,
        onSuccess: (File) -> Unit,
        onFailure: (Exception) -> Unit
    ) {
        _repo.takePicture(
            imageCapture,
            onSuccess,
            onFailure
        )
    }

}