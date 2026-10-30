package com.advance.neuraldigit_ocr.utils

import android.content.Context
import android.graphics.Bitmap
import android.graphics.ImageDecoder
import android.net.Uri

object BitmapFactory {

    fun uriToBitmap(
        context: Context,
        uri: Uri
    ): Bitmap {
        val source = ImageDecoder.createSource(
            context.contentResolver,
            uri
        )

        return ImageDecoder.decodeBitmap(source) { decoder, _, _ ->
            decoder.isMutableRequired = false
        }
    }

}