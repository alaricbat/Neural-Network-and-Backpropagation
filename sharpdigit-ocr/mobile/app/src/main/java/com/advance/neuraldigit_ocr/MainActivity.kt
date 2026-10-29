package com.advance.neuraldigit_ocr

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.material3.MaterialTheme
import com.advance.neuraldigit_ocr.screens.HomeScreen
import com.advance.neuraldigit_ocr.viewmodel.OcrViewModel

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            MaterialTheme {
                enableEdgeToEdge()
                HomeScreen(vm = OcrViewModel())
            }
        }

    }

}