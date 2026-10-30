package com.advance.neuraldigit_ocr.screens

import android.Manifest
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
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
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelStoreOwner
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.advance.neuraldigit_ocr.R
import com.advance.neuraldigit_ocr.factories.ViewModelFactory
import com.advance.neuraldigit_ocr.mvi.CameraEffect
import com.advance.neuraldigit_ocr.mvi.CameraIntent
import com.advance.neuraldigit_ocr.state.CameraPermissionState
import com.advance.neuraldigit_ocr.utils.Logger
import com.advance.neuraldigit_ocr.viewmodel.OcrViewModel

private val TAG = "CameraScreen"

@Composable
fun CameraScreen() {

    val context = LocalContext.current
    val owner = context as ViewModelStoreOwner
    val viewModel = remember(owner) {
        ViewModelProvider(
            owner,
            ViewModelFactory()
        )[OcrViewModel::class.java]
    }

    val permissionLauncher = rememberLauncherForActivityResult(
        contract = ActivityResultContracts.RequestPermission()
    ) { granted ->
        val permissionState = if (granted) {
            CameraPermissionState.GRANTED
        } else {
            CameraPermissionState.DENIED
        }
        viewModel.onPermissionResult(permissionState)
    }
    val uiState by viewModel.uiState.collectAsStateWithLifecycle()

    LaunchedEffect(viewModel) {
        Logger.info(TAG, "[LaunchedEffect]: execute.")

        viewModel.effect.collect { effect ->
            Logger.info(TAG, "[LaunchedEffect]: received => $effect")
            when(effect) {
                CameraEffect.RequestCameraPermission -> {
                    Logger.info(TAG, "[LaunchedEffect]: effect => CameraEffect.RequestCameraPermission")
                    permissionLauncher.launch(Manifest.permission.CAMERA)
                }
                CameraEffect.StartCamera -> {
                    Logger.info(TAG, "[LaunchedEffect]: effect => CameraEffect.StartCamera")

                }
                is CameraEffect.ShowError -> {
                    Logger.info(TAG, "[LaunchedEffect]: effect => CameraEffect.ShowError")
                }
            }
        }
    }

    Column (
        modifier = Modifier
            .fillMaxSize()
            .padding(24.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.spacedBy(16.dp, Alignment.CenterVertically)
    ) {

        when(uiState.cameraPermission) {

            CameraPermissionState.UNKNOWN -> {
                UngrantedComponents(viewModel, R.string.str_camera_is_first_access)
            }

            CameraPermissionState.GRANTED -> {
                GrantedComponents(viewModel)
            }

            CameraPermissionState.DENIED -> {
                UngrantedComponents(viewModel, R.string.str_camera_is_denied)
            }

            CameraPermissionState.PERMANENTLY_DENIED -> {
                UngrantedComponents(viewModel, R.string.str_camera_is_denied)
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

    OutlinedButton (
        modifier = Modifier.fillMaxWidth(),
        onClick =  {
            Logger.info(TAG, "[GrantedComponents][CapturedClick-Button]: execute.")
            viewModel.onIntent(CameraIntent.CapturedClick)
        }
    ) {
        Text(
            text = stringResource(R.string.str_capture_image)
        )
    }
}

@Preview
@Composable
fun PreviewHomeScreen() {
    CameraScreen()
}