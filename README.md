# Neural Network and Backpropagation - SharpDigit OCR

SharpDigit OCR is an Android application and C++ inference engine for recognizing a handwritten digit (0-9) from a camera photo. The repository also contains a NumPy multilayer perceptron (MLP) implementation and Jupyter notebooks for MNIST training, visualization, and image preprocessing.

> **Current camera capability:** the app captures and recognizes one still photo at a time. It does not record video or run continuous, live OCR.

## Contents

- [Project layout](#project-layout)
- [How recognition works](#how-recognition-works)
- [Neural network and training](#neural-network-and-training)
- [Model files](#model-files)
- [Build and run](#build-and-run)
- [Next-version optimization ideas](#next-version-optimization-ideas)

## Project layout

```text
sharpdigit-ocr/
├── ml/
│   ├── notebooks/                 # MNIST training, visualization, preprocessing
│   └── src/
│       ├── clazz/                 # NumPy MLP, ReLU, and softmax
│       ├── model/                 # Trained weights and sample feature matrices
│       └── img/                   # MLP illustration
├── core-ml-engine/
│   ├── include/                   # C++ matrix, model, weight-reader, and CV interfaces
│   ├── src/                       # C++ model, image pipeline, and readers
│   └── assets/                    # Model, sample images, and test matrices
└── mobile/
    ├── app/                       # Android UI, camera flow, and application logic
    └── nativelib/                 # Kotlin/JNI bridge and Android native library
```

The repository root is the documentation and workspace root. Android sources are under `sharpdigit-ocr/mobile`; the desktop C++ engine is under `sharpdigit-ocr/core-ml-engine`; the Python training code is under `sharpdigit-ocr/ml`.

## How recognition works

### Android camera and UI flow

1. `OcrApplication` initializes the app-wide Android `Context`. On screen entry, `CameraScreen` checks the camera permission and starts native model initialization.
2. If permission has not been granted, the screen requests it. With permission, the user opens a CameraX preview using the back camera.
3. CameraX binds a preview and an `ImageCapture` use case to the screen lifecycle. Pressing the capture button writes a JPEG named `OCR_<timestamp>.jpg` in the app's external pictures directory. Before capture, the repository removes older files with that naming pattern.
4. The captured image is shown for review. The user can capture again or request prediction.
5. Prediction loads the saved image as an Android bitmap, converts it to `ARGB_8888`, and calls the JNI method `nativePredict`. Native code converts the pixels to OpenCV BGR, preprocesses the image, and returns the index of the highest-scoring digit class.
6. The UI displays that class as the predicted digit. Back navigation returns to the image preview.

The screen state and one-shot UI effects are held in a `ViewModel` using Kotlin `StateFlow` and a `Channel`. Photo capture is delegated through a use-case/repository; prediction currently calls the JNI bridge directly from the `ViewModel`. Model initialization and prediction run on an IO coroutine dispatcher. Camera preview resources are bound to and released with the lifecycle. The recognition repository and use case are present as placeholders but are not part of the active prediction path.

### Native image preprocessing

The native `ImageProcessor` converts the input photo to a 28x28 feature vector in this order:

1. Convert BGR/BGRA input to grayscale.
2. Apply a 21 × 21 Gaussian blur.
3. Apply an inverse binary threshold using Otsu's threshold selection, making dark digit strokes foreground.
4. Crop a border around the image.
5. Apply a sharpening filter.
6. Keep sufficiently large external contours located within the central image region.
7. Dilate the remaining foreground with a 13 × 13 rectangular kernel.
8. Find the nonzero bounding box, center it on a padded square, resize it to 28 × 28 with area interpolation, and apply a final Otsu threshold.
9. Flatten row by row to a single 784-value matrix. Pixel values are copied as grayscale values (0–255).

The image-processing interfaces are separated from their implementations. A standalone contour cropper is also provided, but the active `ImageProcessor` pipeline uses `ImageFilter`, `ImageResizer`, and `ImageToMatrix`.

### Native model inference

The JNI bridge loads the model once into process-wide state. A mutex protects initialization and prediction. Model initialization reads weight and bias matrices; prediction applies the image pipeline, then performs a forward pass:

```text
784 input features -> dense layer + ReLU -> dense layer + softmax -> argmax digit
```

The C++ engine uses its own row-major `Matrix` type and OpenMP loops for matrix multiplication, bias addition, activations, and image-to-feature conversion. Softmax subtracts the row maximum before exponentiation for numerical stability. The desktop executable can test the model against text feature matrices and sample JPEGs.

## Neural network and training

`ml/src/clazz/MultilayerPerceptron.py` implements the training-side MLP with NumPy:

- Default dimensions: 784 input features, one hidden layer of 128 units, and 10 output classes.
- Hidden layers use ReLU; the output layer uses softmax probabilities.
- Weights are initialized with a positive random distribution scaled by `sqrt(2 / feature_size)`; biases start at zero.
- Backpropagation computes the softmax/cross-entropy gradient (`probabilities - labels`), averages weight and bias gradients over each batch, and propagates through ReLU.
- `fit` shuffles examples at each epoch and uses mini-batch gradient descent. Defaults are 1,000 epochs, learning rate 0.001, and batch size 64.
- `predict` returns the class with the highest output probability.
- `save_model` writes NumPy arrays to a compressed `.npz` archive.

The notebooks load MNIST through TensorFlow/Keras, flatten each 28x28 image to 784 features, train/evaluate the NumPy model, visualize samples, and explore preprocessing captured digit images. TensorFlow is used to retrieve the dataset in the notebook; the MLP's forward pass and backpropagation are implemented in NumPy.

## Model files

The C++ and Android inference readers use paired matrix files named `W_0.bin`, `b_0.bin`, `W_1.bin`, and `b_1.bin`. Each binary file contains two signed 32-bit dimensions (`rows`, then `columns`) followed by row-major 32-bit floating-point values. The Android app bundles these files under `mobile/app/src/main/assets/model/`; `ModelAssetManager` copies them to the app's private files directory before native initialization.

The Python `save_model` method writes `.npz`, not the binary format consumed by C++. Therefore, retraining alone does not update the Android model: the new weights must also be exported into the expected `.bin` format and placed in the Android assets directory. Keep model dimensions, pixel normalization, image preprocessing, and binary serialization consistent between training and inference.

## Build and run

### Android app

Open `sharpdigit-ocr/mobile` in Android Studio and build/run the `app` configuration on an Android 13+ device or emulator (the app's `minSdk` is 33). The app requests camera permission at runtime and requires a camera-capable device or emulator.

From a shell with the Android SDK, NDK, CMake, and required native dependencies configured:

```bash
cd sharpdigit-ocr/mobile
./gradlew :app:assembleDebug
```

The native build currently expects OpenCV's Android SDK at `$HOME/OpenCV-android-sdk/sdk/native/jni` and links the prebuilt `nativelib/src/main/cpp/sharpdigit_sdk/prebuilt/libsharpdigit_ocr.so`. Adjust the native build configuration if your OpenCV SDK or prebuilt library is located elsewhere.

### Desktop C++ engine

The desktop engine requires CMake 3.22+, a C++17 compiler, OpenCV 4.9, and OpenMP. Its CMake configuration currently points to `$HOME/opencv-4.9.0/lib/cmake/opencv4`; update `OpenCV_DIR` in `core-ml-engine/CMakeLists.txt` if needed.

```bash
cd sharpdigit-ocr/core-ml-engine
cmake -S . -B build
cmake --build build
./build/core_ml_engine
```

Run the executable with `core-ml-engine` as the working directory because its sample model and input paths are relative to the current directory.

### Python training and notebooks

Use Python with NumPy to run the MLP module. The notebooks additionally require Jupyter and TensorFlow/Keras for MNIST dataset loading; install those dependencies in your own environment before opening the notebooks. The repository does not currently include a Python dependency manifest.

## Next-version optimization ideas

These are possible improvements, not features currently implemented.

### Camera capture and recording

- **Add an explicit video mode:** use CameraX `VideoCapture` for user-started/stopped recordings, with a clear recording indicator, duration/size limits, and lifecycle-safe cleanup. Keep still-photo capture as the default flow.
- **Add live digit recognition:** use CameraX `ImageAnalysis` to process preview frames without saving a video. Analyze at a bounded rate, keep only the latest frame, and close every `ImageProxy` promptly to avoid backpressure and memory growth.
- **Improve capture quality:** add tap-to-focus/exposure controls, a target guide, and validation for blur, lighting, and whether a digit is in frame before accepting a capture.
- **Handle multiple digits deliberately:** the current classifier returns one class for an image. A future OCR mode should detect and order multiple digit regions rather than treating a whole frame as one digit.

### Deep-learning model

- **Evaluate a small CNN:** compare a convolutional network with the current fully connected MLP on both MNIST and a held-out set of real phone-camera images. A CNN is better suited to spatial image features, but should only replace the MLP if measured accuracy and device latency improve.
- **Train for camera conditions:** add rotations, scale/translation, stroke-width, illumination, blur, and perspective augmentation. Include labeled camera-captured examples and keep train/validation/test data separated by source.
- **Make preprocessing/model contracts explicit:** use the same grayscale polarity, centering, 28 × 28 sizing, and numeric normalization in training and C++; test those transformations against shared golden images.
- **Export and validate automatically:** add a repeatable exporter from trained weights to the C++ binary format (or adopt a supported deployment format such as TensorFlow Lite). Verify tensor shapes, output labels, and parity against Python reference predictions before packaging.
- **Report useful metrics:** track per-class precision/recall, confusion matrices, calibration, model size, and on-device latency instead of relying on example predictions alone.

### Runtime and maintainability

- Benchmark the preprocessing and inference stages separately before optimizing. Then consider reducing full-resolution copies, reusing matrix buffers, and tuning OpenMP/thread use for mobile CPUs.
- Add unit tests for preprocessing edge cases (blank images, off-center digits, multiple contours, and different bitmap formats) and integration tests that compare native predictions with a Python reference.
- Add confidence scores and an explicit low-confidence/retry state so the UI does not present every argmax as equally certain.