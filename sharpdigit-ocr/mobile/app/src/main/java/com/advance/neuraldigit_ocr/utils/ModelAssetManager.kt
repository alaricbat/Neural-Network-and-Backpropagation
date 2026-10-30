package com.advance.neuraldigit_ocr.utils

import android.content.Context
import java.io.File

const val TAG = "ModelAssetManager"

object ModelAssetManager {

    private val modelFiles = listOf(
        "b_0.bin",
        "b_1.bin",
        "W_0.bin",
        "W_1.bin"
    )

    fun prepare(context: Context): String {
        val modelDir = File(context.filesDir, "model")
        if (!modelDir.exists()) {
            modelDir.mkdirs()
        }
        modelFiles.forEach { fileName ->
            val targetFile = File(modelDir, fileName)

            context.assets.open("model/$fileName").use { input ->
                targetFile.outputStream().use { output ->
                    input.copyTo(output)
                }
            }
        }

        Logger.info(TAG, "[prepare]: " + modelDir.absolutePath)
        return modelDir.absolutePath
    }

}