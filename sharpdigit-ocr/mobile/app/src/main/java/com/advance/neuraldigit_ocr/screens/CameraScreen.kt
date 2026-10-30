package com.advance.neuraldigit_ocr.screens

import android.Manifest
import android.annotation.SuppressLint
import android.content.pm.PackageManager
import android.widget.Toast
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.core.content.ContextCompat
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelStoreOwner
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import coil.compose.AsyncImage
import com.advance.neuraldigit_ocr.R
import com.advance.neuraldigit_ocr.factories.ViewModelFactory
import com.advance.neuraldigit_ocr.mvi.CameraEffect
import com.advance.neuraldigit_ocr.mvi.CameraIntent
import com.advance.neuraldigit_ocr.state.CameraScreenState
import com.advance.neuraldigit_ocr.utils.Logger
import com.advance.neuraldigit_ocr.viewmodel.OcrViewModel

private val TAG = "CameraScreen"

@SuppressLint("StateFlowValueCalledInComposition")
@Composable
fun CameraScreen() {

    val context = LocalContext.current

    val owner = context as ViewModelStoreOwner

    val vm = remember(owner) {
        ViewModelProvider(
            owner,
            ViewModelFactory()
        )[OcrViewModel::class.java]
    }

    val permissionLauncher = rememberLauncherForActivityResult(
        contract = ActivityResultContracts.RequestPermission()
    ) { granted ->
        val state = if (granted) {
            CameraScreenState.CAMERA_GRANTED
        } else {
            CameraScreenState.IDLE
        }
        vm.onStateChanged(state)
    }

    val uiState by vm.uiState.collectAsStateWithLifecycle()

    LaunchedEffect(vm) {
        Logger.info(TAG, "[LaunchedEffect]: execute.")

        vm.effect.collect { effect ->
            Logger.info(TAG, "[LaunchedEffect]: received => $effect")
            when (effect) {
                CameraEffect.RequestCameraPermission -> {
                    Logger.info(
                        TAG,
                        "[LaunchedEffect]: effect => CameraEffect.RequestCameraPermission"
                    )
                    permissionLauncher.launch(Manifest.permission.CAMERA)
                }

                CameraEffect.StartCamera -> {
                    Logger.info(TAG, "[LaunchedEffect]: effect => CameraEffect.StartCamera")
                    vm.onStateChanged(CameraScreenState.CAMERA_PREVIEW)
                }

                CameraEffect.StopCamera -> {
                    Logger.info(TAG, "[LaunchedEffect]: effect => CameraEffect.StopCamera")
                    vm.onStateChanged(CameraScreenState.CAMERA_PREVIEW_CANCEL)
                }

                is CameraEffect.ShowError -> {
                    Logger.info(TAG, "[LaunchedEffect]: effect => CameraEffect.ShowError")
                }
            }
        }
    }

    LaunchedEffect(Unit) {
        val granted = ContextCompat.checkSelfPermission(
            context,
            Manifest.permission.CAMERA
        ) == PackageManager.PERMISSION_GRANTED
        val state = if (granted) {
            CameraScreenState.CAMERA_GRANTED
        } else {
            CameraScreenState.IDLE
        }
        vm.onStateChanged(state)
        vm.initModel()
    }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(24.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.spacedBy(16.dp, Alignment.CenterVertically)
    ) {

        when (uiState.cameraScreenState) {
            CameraScreenState.IDLE -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.IDLE")
                UngrantedComponents(vm, R.string.str_camera_idle)
            }
            CameraScreenState.CAMERA_GRANTED -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.CAMERA_GRANTED")
                GrantedComponents(vm)
            }
            CameraScreenState.CAMERA_PREVIEW -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.CAMERA_PREVIEW")
                StartCameraComponents(vm)
            }
            CameraScreenState.CAMERA_PREVIEW_CANCEL -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.CAMERA_PREVIEW_CANCEL")
                if (vm.data.value.capturedImagePath != null) {
                    vm.onStateChanged(CameraScreenState.IMAGE_PREVIEW)
                } else {
                    vm.onStateChanged(CameraScreenState.CAMERA_GRANTED)
                }
            }
            CameraScreenState.CAPTURE_PROCESSING -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.CAPTURE_PROCESSING")
            }
            CameraScreenState.CAPTURE_FINISH -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.CAPTURE_FINISH")
                vm.onStateChanged(CameraScreenState.IMAGE_PREVIEW)
            }
            CameraScreenState.IMAGE_PREVIEW -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.IMAGE_PREVIEW")
                CapturedImageComponents(vm)
            }
            CameraScreenState.OCR_PROCESSING -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.OCR_PROCESSING")
            }
            CameraScreenState.OCR_FINISH -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.OCR_FINISH")
                Toast.makeText(context, vm.data.value.predictedValue, Toast.LENGTH_SHORT)
                vm.onStateChanged(CameraScreenState.IMAGE_PREVIEW)
            }
            CameraScreenState.ERROR -> {
                Logger.info(TAG, "[uiState.cameraScreenState]: CameraScreenState.ERROR")
            }
        }
    }

}

@Composable
private fun UngrantedComponents(viewModel: OcrViewModel, stringRes: Int) {
    Text(
        modifier = Modifier.fillMaxWidth(),
        text = stringResource(stringRes),
        fontSize = 10.sp,
        textAlign = TextAlign.Center
    )

    Button(
        onClick = {
            Logger.info(TAG, "[UngrantedComponents][RequestCameraPermission-Button]: execute.")
            viewModel.onIntent(CameraIntent.RequestCameraPermission)
        }
    ) {
        Text(
            text = stringResource(R.string.str_grant_camera)
        )
    }
}

@Composable
private fun GrantedComponents(viewModel: OcrViewModel) {
    Text(
        modifier = Modifier.fillMaxWidth(),
        text = stringResource(R.string.str_camera_is_granted),
        fontSize = 10.sp,
        textAlign = TextAlign.Center
    )

    OutlinedButton(
        modifier = Modifier.fillMaxWidth(),
        onClick = {
            Logger.info(TAG, "[GrantedComponents][CapturedClick-Button]: execute.")
            viewModel.onIntent(CameraIntent.CapturedClicked)
        }
    ) {
        Text(
            text = stringResource(R.string.str_capture_image)
        )
    }
}

@Composable
private fun StartCameraComponents(vm: OcrViewModel) {
    Column(
        modifier = Modifier.fillMaxSize(),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        CameraPreview(
            modifier = Modifier
                .fillMaxWidth()
                .weight(1f),
            onReady = { imageCapture ->
                vm.setImageCapture(imageCapture)
            },
            onError = { message ->
                Logger.error(TAG, message)
            }
        )
        Button(
            onClick = {
                vm.capturePhoto()
            }
        ) {
            Text(stringResource(R.string.str_capture))
        }
    }
}

@SuppressLint("StateFlowValueCalledInComposition")
@Composable
private fun CapturedImageComponents(vm: OcrViewModel) {

    Column(
        modifier = Modifier.fillMaxSize(),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        AsyncImage(
            model = vm.data.value.capturedImagePath,
            contentDescription = "",
            modifier = Modifier
                .fillMaxWidth()
                .weight(2f),
        )

        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(16.dp)
        ) {
            Button(
                modifier = Modifier.weight(1f),
                onClick = {
                    Logger.info(TAG, "[CapturedImageComponent][PredictClicked-Button]: execute.")
                    vm.onIntent(CameraIntent.PredictClicked)
                }
            ) {
                Text(
                    text = stringResource(R.string.str_predict)
                )
            }
            Button(
                modifier = Modifier.weight(1f),
                onClick = {
                    Logger.info(TAG, "[CapturedImageComponent][CapturedClick-Button]: execute.")
                    vm.onIntent(CameraIntent.CapturedClicked)
                }
            ) {
                Text(
                    text = stringResource(R.string.str_capture_image)
                )
            }
        }
    }

}

@Preview
@Composable
fun PreviewHomeScreen() {
    CameraScreen()
}