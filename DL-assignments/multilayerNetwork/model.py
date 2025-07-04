import numpy as np

from layers import FullyConnectedLayer, ReLULayer, softmax_with_cross_entropy, l2_regularization


class TwoLayerNet:
    """ Neural network with two fully connected layers """

    def __init__(self, n_input, n_output, hidden_layer_size, reg):
        """
        Initializes the neural network

        Arguments:
        n_input, int - dimension of the model input
        n_output, int - number of classes to predict
        hidden_layer_size, int - number of neurons in the hidden layer
        reg, float - L2 regularization strength
        """
        self.reg = reg
        # TODO Create necessary layers
        self.input_layer = FullyConnectedLayer(n_input, hidden_layer_size)
        self.relu = ReLULayer()
        self.output_layer = FullyConnectedLayer(hidden_layer_size, n_output)
        
        self.W_in = self.input_layer.params()['W']
        self.W_out = self.output_layer.params()['W']
        self.B_in = self.input_layer.params()['B']
        self.B_out = self.output_layer.params()['B']

    def compute_loss_and_gradients(self, X, y):
        for _, param in self.params().items():
            param.grad.fill(0)

        to_relu = self.input_layer.forward(X)
        to_output_layer = self.relu.forward(to_relu)
        pred = self.output_layer.forward(to_output_layer)
        loss, dprediction = softmax_with_cross_entropy(pred, y)

        grad_output_layer = self.output_layer.backward(dprediction)
        grad_relu_layer = self.relu.backward(grad_output_layer)
        grad_input_layer = self.input_layer.backward(grad_relu_layer)

        for _, param in self.params().items():
            loss_l2, grad_l2 = l2_regularization(param.value, self.reg)
            loss += loss_l2
            param.grad += grad_l2

        return loss


    def predict(self, X):
        """
        Produces classifier predictions on the set

        Arguments:
          X, np array (test_samples, num_features)

        Returns:
          y_pred, np.array of int (test_samples)
        """
        # TODO: Implement predict
        # Hint: some of the code of the compute_loss_and_gradients
        # can be reused
        to_relu = self.input_layer.forward(X)
        to_output_layer = self.relu.forward(to_relu)
        weights = self.output_layer.forward(to_output_layer)
        pred = np.argmax(weights, axis=-1)
        
        return pred

    def params(self):
        
        result = {'W_out': self.W_out, 'W_in': self.W_in, 
                  'B_out': self.B_out, 'B_in': self.B_in}
        
        return result
