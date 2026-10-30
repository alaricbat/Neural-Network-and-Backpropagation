package com.advance.neuraldigit_ocr.screens

import androidx.activity.compose.BackHandler
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageCapture
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
import androidx.core.content.ContextCompat
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelStoreOwner
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.advance.neuraldigit_ocr.factories.ViewModelFactory
import com.advance.neuraldigit_ocr.mvi.CameraIntent
import com.advance.neuraldigit_ocr.state.CameraScreenState
import com.advance.neuraldigit_ocr.utils.Logger
import com.advance.neuraldigit_ocr.viewmodel.OcrViewModel

private const val TAG = "CameraPreview"

@Composable
fun CameraPreview(
    modifier: Modifier = Modifier,
    onReady: (ImageCapture) -> Unit,
    onError: (String) -> Unit
) {

    val context = LocalContext.current
    val lifecycleOwner = LocalLifecycleOwner.current
    val owner = context as ViewModelStoreOwner
    val vm = remember(owner) {
        ViewModelProvider(
            owner,
            ViewModelFactory()
        )[OcrViewModel::class.java]
    }

    BackHandler {
        if (vm.uiState.value.cameraScreenState == CameraScreenState.CAMERA_PREVIEW) {
            vm.onIntent(CameraIntent.CameraCancel)
        }
    }

    val previewView = remember {
        PreviewView(context).apply {
            scaleType = PreviewView.ScaleType.FILL_CENTER
        }
    }

    DisposableEffect(lifecycleOwner) {

        val cameraProviderFuture =
            ProcessCameraProvider.getInstance(context)

        val executor = ContextCompat.getMainExecutor(context)

        val listener = Runnable {
            try {
                val cameraProvider = cameraProviderFuture.get()

                val preview = Preview.Builder()
                    .build()
                    .also {
                        it.surfaceProvider = previewView.surfaceProvider
                    }

                val imageCapture = ImageCapture.Builder()
                    .setCaptureMode(ImageCapture.CAPTURE_MODE_MINIMIZE_LATENCY)
                    .build()

                cameraProvider.unbindAll()

                cameraProvider.bindToLifecycle(
                    lifecycleOwner,
                    CameraSelector.DEFAULT_BACK_CAMERA,
                    preview,
                    imageCapture
                )

                Logger.info(TAG, "[CameraPreview]: Camera initialized successfully.")

                onReady(imageCapture)

            } catch (exception: Exception) {
                Logger.error(TAG, "[CameraPreview]: Camera initialization failed.", exception)

                onError(
                    "[CameraPreview]: " + (exception.message ?: "Camera initialization failed.")
                )
            }
        }

        cameraProviderFuture.addListener(listener, executor)

        onDispose {
            try {
                if (cameraProviderFuture.isDone) {
                    cameraProviderFuture.get().unbindAll()
                }
            } catch (exception: Exception) {
                Logger.error(TAG, "[CameraPreview]: Camera release failed.", exception)
            }
        }
    }

    AndroidView(
        factory = {
            previewView
        },
        modifier = modifier
    )

}