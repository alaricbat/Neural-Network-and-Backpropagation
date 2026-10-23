import os
import numpy as np
from .ReLU import ReLU
from .Softmax import Softmax

class MultilayerPerceptron:

    def __init__(self,
                 feature_size=784,
                 hidden_size=[128],
                 labels_num=10,
                 epochs=1000,
                 lr=0.001,
                 batch_size = 64):
        
        self.epochs = epochs
        self.lr = lr
        self.batch_size = batch_size

        self.relu = ReLU()
        self.softmax = Softmax()

        self.W = []
        self.b =[]
        self.Z = []
        self.A = []

        num_layers = [feature_size] + hidden_size + [labels_num]

        for i in range(len(num_layers) - 1):
            w = np.random.rand(num_layers[i], num_layers[i + 1]) * np.sqrt(
                2.0 / feature_size
            )
            self.W.append(w)

            b_i = np.zeros((1, num_layers[i + 1]))
            self.b.append(b_i)


    def forward(self, X):

        self.Z = []
        self.A = [X]

        self.X = X
        current_input = X

        num_weights = len(self.W)

        for i in range(num_weights):

            z = current_input @ self.W[i] + self.b[i]
            self.Z.append(z)

            if i == num_weights - 1:
                out = self.softmax.forward(z)
                self.A.append(out)
                return out
            else:
                out = self.relu.forward(z)
                self.A.append(out)
                current_input = out

        return self.A[-1]

    def backward(self, y):

        N = self.X.shape[0]
        num_layers = len(self.W)

        dW = [None] * num_layers
        db = [None] * num_layers

        dJ_dZ = self.softmax.backward(y)

        for i in range(num_layers - 1, -1, -1):

            dW[i] = (self.A[i].T @ dJ_dZ) / N
            db[i] = np.sum(dJ_dZ, axis=0, keepdims=True) / N

            if i > 0:
                dA = np.dot(dJ_dZ, self.W[i].T)  
                self.relu.Z = self.Z[i - 1]
                dJ_dZ = self.relu.backward(dA)

        return dW, db


    def fit(self, X, y):

        num_samples = X.shape[0]
        
        for _ in range(self.epochs):

            indices = np.arange(num_samples)
            np.random.shuffle(indices)
            X_shuffled = X[indices]
            y_shuffled = y[indices]

            num_batches = int(np.ceil(num_samples/self.batch_size))

            for i in range(num_batches):

                start_idx = i * self.batch_size
                end_idx = min(start_idx + self.batch_size, num_samples)

                X_batch = X_shuffled[start_idx:end_idx]
                y_batch = y_shuffled[start_idx:end_idx]

                self.forward(X_batch)
                dW, db = self.backward(y_batch)

                for j in range(len(self.W)):
                    self.W[j] -= self.lr * dW[j]
                    self.b[j] -= self.lr * db[j]

    def predict(self, X):
        output = self.forward(X)
        return np.argmax(output, axis=1)

    def save_model(self, filepath="../model/mlp_model.npz"):
        data_to_save = {}
        
        dir_name = os.path.dirname(filepath)
        if dir_name and not os.path.exists(dir_name):
            os.makedirs(dir_name, exist_ok=True)
        
        for i, w in enumerate(self.W):
            data_to_save[f'W_{i}'] = w
            
        for i, b in enumerate(self.b):
            data_to_save[f'b_{i}'] = b
            
        np.savez_compressed(filepath, **data_to_save)
