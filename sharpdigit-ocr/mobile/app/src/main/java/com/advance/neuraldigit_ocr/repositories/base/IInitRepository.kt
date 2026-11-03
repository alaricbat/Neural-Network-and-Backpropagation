package com.advance.neuraldigit_ocr.repositories.base


interface IInitRepository {

    fun init(
        onSuccess: () -> Unit,
        onFailure: (Exception) -> Unit
    )

}