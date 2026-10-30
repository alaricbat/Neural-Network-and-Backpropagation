package com.advance.neuraldigit_ocr.viewmodel

import android.graphics.Bitmap
import android.net.Uri
import androidx.camera.core.ImageCapture
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.advance.neuraldigit_ocr.data.OcrData
import com.advance.neuraldigit_ocr.mvi.CameraEffect
import com.advance.neuraldigit_ocr.mvi.CameraIntent
import com.advance.neuraldigit_ocr.mvi.CameraUiState
import com.advance.neuraldigit_ocr.state.CameraScreenState
import com.advance.neuraldigit_ocr.usecase.TakeImageUseCase
import com.advance.neuraldigit_ocr.utils.BitmapFactory
import com.advance.neuraldigit_ocr.utils.Context
import com.advance.neuraldigit_ocr.utils.Logger
import com.advance.neuraldigit_ocr.utils.ModelAssetManager
import com.advance.neuraldigit_ocr.utils.NativeOcr
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.receiveAsFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import java.io.File

private const val TAG = "OcrViewModel"

class OcrViewModel() : ViewModel(), IViewModelBehavior {

    private val _uiState = MutableStateFlow(CameraUiState())
    val uiState = _uiState.asStateFlow()

    private val _effect = Channel<CameraEffect>()
    val effect = _effect.receiveAsFlow()

    private val _data = MutableStateFlow(OcrData())
    val data = _data.asStateFlow()

    private val _takeImageUseCase = TakeImageUseCase()

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

            CameraIntent.CapturedClicked -> {
                Logger.info(
                    TAG,
                    "[onIntent]: intent => CapturedClick"
                )
                viewModelScope.launch {
                    _effect.send(CameraEffect.StartCamera)
                }
            }

            CameraIntent.PredictClicked -> {
                Logger.info(
                    TAG,
                    "[onIntent]: intent => PredictClicked"
                )
                viewModelScope.launch(Dispatchers.IO) {
                    try {
                        val uri = _data.value.capturedImagePath
                            ?: throw IllegalStateException("Image URI is null")
                        val bitmap = BitmapFactory.uriToBitmap(Context.get(), uri)
                        Logger.info(TAG, "[onIntent][PredictClicked]: Bitmap width: ${bitmap.width}")
                        Logger.info(TAG, "[onIntent][PredictClicked]: Bitmap height: ${bitmap.height}")
                        Logger.info(TAG, "[onIntent][PredictClicked]: Bitmap config: ${bitmap.config}")
                        Logger.info(TAG, "[onIntent][PredictClicked]: Bitmap mutable: ${bitmap.isMutable}")
                        Logger.info(TAG, "[onIntent][PredictClicked]: Bitmap recycled: ${bitmap.isRecycled}")
                        val inputBitmap = bitmap.copy(
                            Bitmap.Config.ARGB_8888,
                            false
                        ) ?: throw IllegalStateException("Cannot convert Bitmap to ARGB_8888")

                        val result = NativeOcr.nativePredict(inputBitmap)
                        Logger.info(TAG, "[onIntent][PredictClicked]: Result: ${result}")
                        _data.update {
                            it.copy(
                                predictedValue = result
                            )
                        }
                        onStateChanged(CameraScreenState.OCR_FINISH)
                    } catch (e: Exception) {
                        Logger.error(TAG, "[onIntent][PredictClicked]: ", e)
                    }
                }
            }

            CameraIntent.CameraCancel -> {
                Logger.info(
                    TAG,
                    "[onIntent]: intent => CameraCancel"
                )
                viewModelScope.launch {
                    _effect.send(CameraEffect.StopCamera)
                }
            }
        }
    }

    override fun onStateChanged(state: CameraScreenState) {
        Logger.info(TAG, "[onPermissionResult]: execute.")
        _uiState.update {
            it.copy(
                cameraScreenState = state
            )
        }
    }

    override fun updateCapturedImage(path: String) {
        Logger.info(TAG, "[onCameraCapturing]: execute.")
        _data.update { it.copy(
            capturedImagePath = Uri.fromFile(File(path))
        ) }
    }

    override fun capturePhoto() {
        Logger.info(TAG, "[capturePhoto]: execute.")
        _takeImageUseCase(
            imageCapture = _data.value.imageCapture!!,
            onSuccess = { file ->
                Logger.info(TAG, "[capturePhoto]: executed successfully with file : ${file.absolutePath}")
                updateCapturedImage(file.absolutePath)
                onStateChanged(CameraScreenState.CAPTURE_FINISH)
            },
            onFailure = { exception ->
                Logger.error(TAG, "[capturePhoto]: ", exception)
            }
        )
    }

    override fun initModel() {
        Logger.info(TAG, "[initModel]: execute.")
        viewModelScope.launch(Dispatchers.IO) {
            try {
                val modelDir = ModelAssetManager.prepare(Context.get())
                val success = NativeOcr.nativeInit(modelDir)
                if (!success) {
                    Logger.error(TAG, "[initModel]: Failed to initialize native OCR model.")
                }
            } catch (e: Exception) {
                Logger.error(TAG, "[initModel]: Native OCR initialization failed.", e)
            }
        }
    }


}