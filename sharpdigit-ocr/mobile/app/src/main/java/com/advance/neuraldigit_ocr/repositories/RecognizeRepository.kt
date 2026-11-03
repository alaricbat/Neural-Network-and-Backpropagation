package com.advance.neuraldigit_ocr.repositories

import android.graphics.Bitmap
import android.net.Uri
import com.advance.neuraldigit_ocr.repositories.base.BaseRepository
import com.advance.neuraldigit_ocr.repositories.base.IRecognizeRepository
import com.advance.neuraldigit_ocr.utils.BitmapFactory
import com.advance.neuraldigit_ocr.utils.Context
import com.advance.neuraldigit_ocr.utils.Logger
import com.advance.neuraldigit_ocr.utils.NativeOcr

class RecognizeRepository: BaseRepository(), IRecognizeRepository {

    private val TAG = "RecognizeRepository"

    override fun predictPhoto(
        uri: Uri,
        onSuccess: (Int) -> Unit,
        onFailure: (Exception) -> Unit
    ) {
        Logger.info(TAG, "[predictPhoto]: execute.")

        try {
            val bitmap = BitmapFactory.uriToBitmap(Context.get(), uri)
            Logger.info(TAG, "[predictPhoto]: Bitmap width: ${bitmap.width}")
            Logger.info(TAG, "[predictPhoto]: Bitmap height: ${bitmap.height}")
            Logger.info(TAG, "[predictPhoto]: Bitmap config: ${bitmap.config}")
            Logger.info(TAG, "[predictPhoto]: Bitmap mutable: ${bitmap.isMutable}")
            Logger.info(TAG, "[predictPhoto]: Bitmap recycled: ${bitmap.isRecycled}")
            val inputBitmap = bitmap.copy(
                Bitmap.Config.ARGB_8888,
                false
            ) ?: throw IllegalStateException("Cannot convert Bitmap to ARGB_8888")
            val result = NativeOcr.nativePredict(inputBitmap)
            Logger.info(TAG, "[predictPhoto]: Result: $result")
            onSuccess(result)
        } catch (e: Exception) {
            onFailure(e)
        }

    }

}