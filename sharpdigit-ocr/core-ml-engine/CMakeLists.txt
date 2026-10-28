cmake_minimum_required(VERSION 3.27)

project(core_ml_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(OpenCV_DIR "$ENV{HOME}/opencv-4.9.0/lib/cmake/opencv4")

find_package(OpenMP REQUIRED)
find_package(OpenCV 4.9 REQUIRED COMPONENTS
    core imgproc imgcodecs
)

add_executable(core_ml_engine
        main.cpp
        include/ml/Matrix.h
        src/ml/Matrix.cpp
        include/ml/interface/IActivation.h
        include/ml/ReLU.h
        src/ml/ReLU.cpp
        include/ml/Softmax.h
        src/ml/Softmax.cpp
        include/ml/MultilayerPerception.h
        src/ml/MultilayerPerceptron.cpp
        include/ml/interface/IMatrixReader.h
        include/ml/reader/TxtMatrixReader.h
        src/ml/TxtMatrixReader.cpp
        include/ml/weight/ModelWeight.h
        include/ml/weight/ModelWeightReader.h
        src/ml/ModelWeightReader.cpp
        include/ml/reader/BinaryMatrixReader.h
        src/ml/BinaryMatrixReader.cpp
        include/cv/interface/IImageFilter.h
        include/cv/interface/IDigitCropper.h
        include/cv/interface/IImageResizer.h
        include/cv/interface/IImageToMatrix.h
        include/cv/ImageResizer.h
        src/cv/ImageResizer.cpp
        include/cv/ImageProcessor.h
        src/cv/ImageFilter.cpp
        include/cv/ImageFilter.h
        include/cv/ContourDigitCropper.h
        src/cv/ContourDigitCropper.cpp
        include/cv/ImageToMatrix.h
        src/cv/ImageToMatrix.cpp
        src/cv/ImageProcessor.cpp
)

target_include_directories(core_ml_engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(core_ml_engine PRIVATE
        OpenMP::OpenMP_CXX
)
