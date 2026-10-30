package com.advance.neuraldigit_ocr.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.advance.neuraldigit_ocr.mvi.CameraEffect
import com.advance.neuraldigit_ocr.mvi.CameraIntent
import com.advance.neuraldigit_ocr.mvi.HomeUiState
import com.advance.neuraldigit_ocr.state.CameraPermissionState
import com.advance.neuraldigit_ocr.utils.Logger
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.receiveAsFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

class OcrViewModel : ViewModel(), IViewModelBehavior {

    private val TAG = "OcrViewModel"

    private val _uiState = MutableStateFlow(HomeUiState())
    val uiState = _uiState.asStateFlow()

    private val _effect = Channel<CameraEffect>()
    val effect = _effect.receiveAsFlow()

    override fun onIntent(intent: CameraIntent) {
        Logger.info(TAG, "[onIntent]: execute.")
        when(intent) {

            CameraIntent.RequestCameraPermission -> {
                Logger.info(
                    TAG,
                    "[onIntent]: intent => RequestCameraPermission"
                )
                viewModelScope.launch {
                    _effect.send(
                        CameraEffect.RequestCameraPermission
                    )
                }

            }

            CameraIntent.CapturedClick -> {
                Logger.info(
                    TAG,
                    "[onIntent]: intent => CapturedClick"
                )
                viewModelScope.launch {
                    _effect.send(CameraEffect.StartCamera)
                }
            }

            CameraIntent.RetryClicked -> {
                Logger.info(TAG, "[onIntent]: intent => RetryClicked")
            }
        }
    }

    override fun onPermissionResult(cameraPermissionState: CameraPermissionState) {
        Logger.info(TAG, "[onPermissionResult]: execute.")
        _uiState.update {
            it.copy(
                cameraPermission = cameraPermissionState,
                isCameraReady = cameraPermissionState == CameraPermissionState.GRANTED
            )
        }
    }


}