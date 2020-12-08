from sigmoid import sigmoid
import numpy as np


def gradient_function(theta, X, y):
    """
    Compute gradient for logistic regression w.r.t. to the parameters theta.

    Args:
        theta: Parameters of shape [num_features]
        X: Data matrix of shape [num_data, num_features]
        y: Labels corresponding to X of size [num_data, 1]

    Returns:
        grad: The gradient of the log-likelihood w.r.t. theta

    """

    grad = None
    #######################################################################
    # TODO:                                                               #
    # Compute the gradient for a particular choice of theta.              #
    # Compute the partial derivatives and set grad to the partial         #
    # derivatives of the cost w.r.t. each parameter in theta              #
    #                                                                     #
    #######################################################################
    
    theta = theta[:,np.newaxis]
    h = sigmoid(np.dot(theta.T, X.T))
    grad = (y - h) * X.T

    #######################################################################
    #                         END OF YOUR CODE                            #
    #######################################################################
    return grad