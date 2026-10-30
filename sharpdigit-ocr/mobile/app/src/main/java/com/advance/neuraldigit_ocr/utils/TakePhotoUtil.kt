package com.advance.neuraldigit_ocr.utils

import android.content.Context
import android.os.Environment
import androidx.camera.core.ImageCapture
import androidx.camera.core.ImageCaptureException
import androidx.core.content.ContextCompat
import java.io.File

private const val TAG = "TakePhotoUtil"

object TakePhotoUtil {

    fun takePhoto(
        context: Context,
        imageCapture: ImageCapture
    ) {

        Logger.info(TAG, "[takePhoto]: execute.")

        val photoFile = File(
            context.getExternalFilesDir(Environment.DIRECTORY_PICTURES),
            "OCR_${System.currentTimeMillis()}.jpg"
        )

        val outputOptions = ImageCapture.OutputFileOptions
            .Builder(photoFile)
            .build()

        imageCapture.takePicture(
            outputOptions,
            ContextCompat.getMainExecutor(context),
            object : ImageCapture.OnImageSavedCallback {
                override fun onImageSaved(outputFileResults: ImageCapture.OutputFileResults) {
                    Logger.info(TAG, "[onImageSaved]: execute.")
                }

                override fun onError(exception: ImageCaptureException) {
                    Logger.error(TAG, "[onError]: execute.")
                }
            }
        )
    }

}