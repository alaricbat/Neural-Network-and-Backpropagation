cmake_minimum_required(VERSION 3.27)

project(core_ml_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

add_executable(core_ml_engine
        main.cpp
        include/Matrix.h
        src/Matrix.cpp
        include/interface/IActivation.h
        include/ReLU.h
        src/ReLU.cpp
        include/Softmax.h
        src/Softmax.cpp
        include/MultilayerPerception.h
        src/MultilayerPerceptron.cpp
        include/interface/IMatrixReader.h
        include/reader/TxtMatrixReader.h
        src/TxtMatrixReader.cpp
        include/weight/ModelWeight.h
        include/weight/ModelWeightReader.h
        src/ModelWeightReader.cpp
        include/reader/BinaryMatrixReader.h
        src/BinaryMatrixReader.cpp
)

target_include_directories(core_ml_engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(core_ml_engine PRIVATE
        OpenMP::OpenMP_CXX
)
