#ifndef CORE_ML_ENGINE_TXTMATRIXREADER_H
#define CORE_ML_ENGINE_TXTMATRIXREADER_H

#include "ml/interface/IMatrixReader.h"

class TxtMatrixReader : public IMatrixReader {

public:
    explicit TxtMatrixReader(int expected_features = 784);

    [[nodiscard]]
    Matrix read(const std::string& file_path) const override;

private:
    int expected_features_;
};

#endif //CORE_ML_ENGINE_TXTMATRIXREADER_H