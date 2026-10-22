import numpy as np

class ReLU:

    def __init__(self):
        self.Z = None

    def forward(self, Z):
        self.Z = Z
        return np.maximum(0, Z)

    def backward(self, dA):
        dZ = np.where(self.Z > 0, dA, 0)
        return dZ
        