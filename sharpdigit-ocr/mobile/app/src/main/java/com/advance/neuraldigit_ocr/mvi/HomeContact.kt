package com.advance.neuraldigit_ocr.mvi

data class HomeUiState(
    val errorMessage: String? = null
)

sealed class HomeIntent {

}

sealed class HomeEffect {

}