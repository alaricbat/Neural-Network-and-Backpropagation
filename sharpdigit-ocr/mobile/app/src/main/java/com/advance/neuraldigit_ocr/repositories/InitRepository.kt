package com.advance.neuraldigit_ocr.repositories

import com.advance.neuraldigit_ocr.repositories.base.BaseRepository
import com.advance.neuraldigit_ocr.repositories.base.IInitRepository
import com.advance.neuraldigit_ocr.utils.Context
import com.advance.neuraldigit_ocr.utils.Logger
import com.advance.neuraldigit_ocr.utils.ModelAssetManager
import com.advance.neuraldigit_ocr.utils.NativeOcr

class InitRepository : BaseRepository(), IInitRepository {

    private val TAG = "InitRepository"

    override fun init(
        onSuccess: () -> Unit,
        onFailure: (Exception) -> Unit) {
        Logger.info(TAG, "[initModel]: execute.")
        try {
            val modelDir = ModelAssetManager.prepare(Context.get())
            val success = NativeOcr.nativeInit(modelDir)
            if (!success) {
                throw RuntimeException("Failed to initialize native OCR model.")
            }
            onSuccess()
        } catch (e: Exception) {
            onFailure(e)
        }
    }


}