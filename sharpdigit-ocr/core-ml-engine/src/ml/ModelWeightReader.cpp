#include "ml/weight/ModelWeightReader.h"
#include "ml/reader/BinaryMatrixReader.h"

ModelWeights ModelWeightReader::read(
        const std::filesystem::path &model_dir
        ) {

    ModelWeights model;

    for (int i = 0; i < 2; i++) {
        const auto weight_path =
                model_dir / ("W_" + std::to_string(i) + ".bin");

        if (!std::filesystem::exists(weight_path)) {
            break;
        }

        model.weights.push_back(
                BinaryMatrixReader::read(weight_path.string()));

        const auto bias_path =
                model_dir / ("b_" + std::to_string(i) + ".bin");

        if (!std::filesystem::exists(bias_path)) {
            throw std::runtime_error(
                    "[ModelWeightReader][read]: Missing bias file: " + bias_path.string()
            );
        }
        model.biases.push_back(
                BinaryMatrixReader::read(bias_path.string()));
    }

    if (model.weights.empty()) {
        throw std::runtime_error("[ModelWeightReader][read]: No model weights found.");
    }

    return model;
}