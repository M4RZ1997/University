from sigmoid import sigmoid
import numpy as np


def predict_function(theta, X, y=None):
    """
    Compute predictions on X using the parameters theta. If y is provided
    computes and returns the accuracy of the classifier as well.

    """

    preds = None
    accuracy = None
    #######################################################################
    # TODO:                                                               #
    # Compute predictions on X using the parameters theta.                #
    # If y is provided compute the accuracy of the classifier as well.    #
    #                                                                     #
    #######################################################################
    
    theta = theta[:, np.newaxis]
    preds = np.squeeze(np.round(sigmoid(np.dot(theta.T, X.T))).astype(int))

    if not(y is None):
        accuracyCount = 0.0
        totalCount = 0.0
        for i in range(y.size):
            totalCount = totalCount + 1
            if preds[i] == y[i]:
                accuracyCount = accuracyCount + 1
        accuracy = accuracyCount / totalCount

    #######################################################################
    #                         END OF YOUR CODE                            #
    #######################################################################
    return preds, accuracy