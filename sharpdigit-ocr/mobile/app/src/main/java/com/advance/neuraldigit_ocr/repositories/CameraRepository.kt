package com.advance.neuraldigit_ocr.repositories

import android.os.Environment
import androidx.camera.core.ImageCapture
import androidx.camera.core.ImageCaptureException
import androidx.core.content.ContextCompat
import com.advance.neuraldigit_ocr.repositories.base.BaseRepository
import com.advance.neuraldigit_ocr.repositories.base.ICameraRepository
import com.advance.neuraldigit_ocr.utils.Context
import com.advance.neuraldigit_ocr.utils.Logger
import java.io.File

private const val TAG = "CameraRepository"

class CameraRepository : BaseRepository(), ICameraRepository {

    override fun takePicture(
        imageCapture: ImageCapture,
        onImgSaved: (File) -> Unit,
        onError: (ImageCaptureException) -> Unit
    ) {
        Logger.info(TAG, "[takePhoto]: execute.")

        val photoFile = File(
            Context.get().getExternalFilesDir(Environment.DIRECTORY_PICTURES),
            "OCR_${System.currentTimeMillis()}.jpg"
        )

        val outputOptions = ImageCapture.OutputFileOptions
            .Builder(photoFile)
            .build()

        imageCapture.takePicture(
            outputOptions,
            ContextCompat.getMainExecutor(Context.get()),
            object : ImageCapture.OnImageSavedCallback {
                override fun onImageSaved(outputFileResults: ImageCapture.OutputFileResults) {
                    Logger.info(TAG, "[onImageSaved]: execute.")
                    onImgSaved(photoFile)
                }

                override fun onError(exception: ImageCaptureException) {
                    Logger.error(TAG, "[onError]: execute.")
                    onError(exception)
                }
            }
        )
    }


}