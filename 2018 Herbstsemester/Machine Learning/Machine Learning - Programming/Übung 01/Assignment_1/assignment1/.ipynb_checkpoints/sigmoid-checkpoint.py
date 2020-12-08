from numpy import exp


def sigmoid(z):
    """
    Computes the sigmoid of z element-wise.

    Args:
        z: An np.array of arbitrary shape

    Returns:
        g: An np.array of the same shape as z

    """

    g = None
    #######################################################################
    # TODO:                                                               #
    # Compute and return the sigmoid of z in g                            #
    #######################################################################

    g = 1 / (1 + exp(-z))

    #######################################################################
    #                         END OF YOUR CODE                            #
    #######################################################################
    return g
