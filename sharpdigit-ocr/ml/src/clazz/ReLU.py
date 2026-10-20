import numpy as np

class ReLU:

    def __init__(self):
        self.Z = None

    def forward(self, Z):
        self.Z = Z
        return np.maximum(0, Z)

    def backward(self, dZ):
        pass
        