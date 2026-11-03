package com.advance.neuraldigit_ocr.repositories.base

import android.net.Uri

interface IRecognizeRepository {

    fun predictPhoto(
        uri: Uri,
        onSuccess: (Int) -> Unit,
        onFailure: (Exception) -> Unit
    )

}