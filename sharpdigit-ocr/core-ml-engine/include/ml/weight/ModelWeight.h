#ifndef CORE_ML_ENGINE_MODELWEIGHT_H
#define CORE_ML_ENGINE_MODELWEIGHT_H

#include "ml/Matrix.h"
#include <vector>

struct ModelWeights {
    std::vector<Matrix> weights;
    std::vector<Matrix> biases;
};

#endif //CORE_ML_ENGINE_MODELWEIGHT_H
