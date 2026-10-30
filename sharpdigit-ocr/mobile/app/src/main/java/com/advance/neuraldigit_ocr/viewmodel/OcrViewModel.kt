package com.advance.neuraldigit_ocr.viewmodel

import androidx.camera.core.ImageCapture
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.advance.neuraldigit_ocr.data.OcrData
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

private const val TAG = "OcrViewModel"

class OcrViewModel : ViewModel(), IViewModelBehavior {

    private val _uiState = MutableStateFlow(HomeUiState())
    val uiState = _uiState.asStateFlow()

    private val _effect = Channel<CameraEffect>()
    val effect = _effect.receiveAsFlow()

    private val _data = MutableStateFlow(OcrData())
    val data = _data.asStateFlow()

    fun setImageCapture(imageCapture: ImageCapture?) {
        _data.update {
            it.copy(imageCapture = imageCapture)
        }
    }

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
                Logger.info(
                    TAG,
                    "[onIntent]: intent => RetryClicked"
                )
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

    override fun onCameraStarted() {
        Logger.info(TAG, "[onCameraStarted]: execute.")
        _uiState.update {
            it.copy(
                isCameraStarted = true
            )
        }
    }

    override fun onCameraClosed() {
        Logger.info(TAG, "[onCameraClosed]: execute.")
        _uiState.update {
            it.copy(
                isCameraStarted = false
            )
        }
    }


}