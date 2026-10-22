import numpy as np

class Softmax:

    def __init__(self):
        self.output = None

    def forward(self, Z):
        exp_Z = np.exp(Z - np.max(Z, axis=1, keepdims=True))
        self.output = exp_Z / np.sum(Z, axis=1, keepdims=True)
        return self.output

    def backward(self, y):
        return self.output - y