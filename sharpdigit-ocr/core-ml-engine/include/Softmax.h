#ifndef CORE_ML_ENGINE_SOFTMAX_H
#define CORE_ML_ENGINE_SOFTMAX_H

#include "interface/IActivation.h"

class Softmax: public IActivation {

private:
    Matrix lastOut;

public:
    Softmax();
    Matrix forward(const Matrix& Z) override;
    Matrix backward(const Matrix& y_one_hot) override;
};

#endif //CORE_ML_ENGINE_SOFTMAX_H
