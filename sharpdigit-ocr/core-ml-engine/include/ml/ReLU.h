#ifndef CORE_ML_ENGINE_RELU_H
#define CORE_ML_ENGINE_RELU_H

#include "Matrix.h"
#include "ml/interface/IActivation.h"

class ReLU : public IActivation {

private:
    Matrix lastZ;

public:
    ReLU();
    Matrix forward(const Matrix &Z) override;
    Matrix backward(const Matrix &dA) override;
};

#endif //CORE_ML_ENGINE_RELU_H
