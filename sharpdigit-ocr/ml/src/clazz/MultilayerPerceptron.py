import numpy as np
from ReLU import ReLU
from Softmax import Softmax

class MultilayerPerceptron:

    def __init__(self,
                 feature_size=784,
                 hidden_size=[128],
                 labels_num=10,
                 epochs=10,
                 lr=0.001):
        
        self.epochs = epochs
        self.lr = lr

        self.relu = ReLU()
        self.softmax = Softmax()

        self.W = []
        self.b =[]
        self.Z = []
        self.A = []

        self.layers = [feature_size] + hidden_size + [labels_num]

        for i in range(self.layers - 1):
            w = np.random.rand((self.layers[i], self.layers[i + 1])) * np.sqrt(
                2.0 / feature_size
            )
            self.W.append(w)

            b_i = np.zeros((1, self.layers[i + 1]))
            self.b.append(b_i)


    def forward(self, X):

        for i in range(self.layers - 1):
            if i == 0:
                z = self.W[i] @ X + self.b[i]
                self.A.append(self.relu.forward(z))
            else:
                z = self.W[i] @ self.A[i-1] + self.b[i]
                self.A.append(self.softmax.forward(z))

        return self.A[-1]

    def backward(self, X, y, output):

        N = self.X.shape[0]
        



    def fit(self, X, y):
        pass

    def predict(self, X):
        pass
