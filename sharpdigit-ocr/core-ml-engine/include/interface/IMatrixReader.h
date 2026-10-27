#ifndef CORE_ML_ENGINE_IMATRIXREADER_H
#define CORE_ML_ENGINE_IMATRIXREADER_H

#include "../Matrix.h"

class IMatrixReader {
public:
    virtual ~IMatrixReader() = default;

    [[nodiscard]]
    virtual Matrix read(const std::string& file_path) const = 0;
};

#endif //CORE_ML_ENGINE_IMATRIXREADER_H
