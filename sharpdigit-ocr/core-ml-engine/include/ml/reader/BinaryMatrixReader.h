
#ifndef CORE_ML_ENGINE_BINARYMATRIXREADER_H
#define CORE_ML_ENGINE_BINARYMATRIXREADER_H

#include "filesystem"
#include "ml/Matrix.h"

class BinaryMatrixReader {
public:
    [[nodiscard]]
    static Matrix read(const std::filesystem::path& file_path);
};

#endif //CORE_ML_ENGINE_BINARYMATRIXREADER_H
