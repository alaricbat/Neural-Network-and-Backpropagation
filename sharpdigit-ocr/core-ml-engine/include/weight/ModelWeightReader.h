#ifndef CORE_ML_ENGINE_MODELWEIGHTREADER_H
#define CORE_ML_ENGINE_MODELWEIGHTREADER_H

#include "ModelWeight.h"
#include <filesystem>

class ModelWeightReader {
public:
    [[nodiscard]]
    static ModelWeights read(const std::filesystem::path& model_dir);
};

#endif //CORE_ML_ENGINE_MODELWEIGHTREADER_H
