import numpy as np

class Softmax:

    def __init__(self):
        self.output = None

    def forward(self, Z):
        exp_Z = np.exp(Z - np.max(Z, axis=1, keepdims=True))
        self.output = exp_Z / np.sum(exp_Z, axis=1, keepdims=True)
        return self.output

    def backward(self, y):
        if y.ndim == 1:
            y_one_hot = np.zeros_like(self.output)
            y_one_hot[np.arange(len(y)), y] = 1
            y = y_one_hot
        return self.output - y