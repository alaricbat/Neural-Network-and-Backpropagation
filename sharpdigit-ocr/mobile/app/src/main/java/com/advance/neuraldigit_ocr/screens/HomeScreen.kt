package com.advance.neuraldigit_ocr.screens

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.advance.neuraldigit_ocr.R
import com.advance.neuraldigit_ocr.viewmodel.OcrViewModel

@Composable
fun HomeScreen(
    vm: OcrViewModel
) {

    Column (
        modifier = Modifier
            .fillMaxSize()
            .padding(24.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        OutlinedButton (
            modifier = Modifier.fillMaxWidth(),
            onClick =  {

            }
        ) {
            Text(
                text = stringResource(R.string.str_capture_image)
            )
        }
    }

}

@Preview
@Composable
fun PreviewHomeScreen() {
    HomeScreen(
        vm = OcrViewModel()
    )
}