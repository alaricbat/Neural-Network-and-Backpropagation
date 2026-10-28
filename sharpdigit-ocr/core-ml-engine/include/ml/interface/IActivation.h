#ifndef CORE_ML_ENGINE_IACTIVATION_H
#define CORE_ML_ENGINE_IACTIVATION_H

#include "ml/Matrix.h"

class IActivation {
public:
    virtual ~IActivation() = default;
    [[nodiscard]] virtual Matrix forward(const Matrix& Z) = 0;
    [[nodiscard]] virtual Matrix backward(const Matrix& upstream_grad) = 0;
};

#endif //CORE_ML_ENGINE_IACTIVATION_H
