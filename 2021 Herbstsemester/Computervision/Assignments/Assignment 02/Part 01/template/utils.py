import random

import numpy as np
from scipy.linalg import null_space
import matplotlib.pyplot as plt
import numpy.linalg


def get_normalization_matrix(x):
    """
    get_normalization_matrix Returns the transformation matrix used to normalize
    the inputs x
    Normalization corresponds to subtracting mean-position and positions
    have a mean distance of sqrt(2) to the center
    """
    # Input: x 3*N
    # 
    # Output: T 3x3 transformation matrix of points

    # TODO TASK:
    # --------------------------------------------------------------
    # Estimate transformation matrix used to normalize
    # the inputs x
    # --------------------------------------------------------------

    # Get centroid and mean-distance to centroid
    T = np.zeros((3, 3))

    # Compute Centroid
    centroid = np.mean(x, 1)

    s = 1 / np.sqrt(np.sum(np.square(np.apply_along_axis(lambda v: v - centroid, 0, x))) / (2 * x.shape[1]))

    T[0, 0] = s
    T[0, 2] = - s * centroid[0]
    T[1, 1] = s
    T[1, 2] = - s * centroid[1]
    T[2, 2] = 1

    """ Test for correctness of squared mean distance equals 2
    norm_x = np.apply_along_axis(lambda v: np.matmul(T, v), 0, x)
    mean = np.zeros((344, 1))
    for i in range(norm_x.shape[1]):
        mean[i] = np.sum(np.square(norm_x[:2, i]))
    print(np.mean(mean))
    """

    return T


def eight_points_algorithm(x1, x2, normalize=True):
    """
    Calculates the fundamental matrix between two views using the normalized 8 point algorithm
    Inputs:
                    x1      3xN     homogeneous coordinates of matched points in view 1
                    x2      3xN     homogeneous coordinates of matched points in view 2
    Outputs:
                    F       3x3     fundamental matrix
    """
    N = x1.shape[1]

    if N < 8:  # not enough entries for eight-point-algorithm
        return

    if normalize:
        # Construct transformation matrices to normalize the coordinates
        # TODO
        Tx1 = get_normalization_matrix(x1)
        Tx2 = get_normalization_matrix(x2)

        # Normalize inputs
        # TODO
        norm_x1 = np.apply_along_axis(lambda x: np.matmul(Tx1, x), 0, x1)
        norm_x2 = np.apply_along_axis(lambda x: np.matmul(Tx2, x), 0, x2)

    # Construct matrix A encoding the constraints on x1 and x2
    # TODO

    A = np.zeros((x1.shape[1], 9))
    for i in range(x1.shape[1]):
        A[i, 0] = norm_x2[0, i] * norm_x1[0, i]
        A[i, 1] = norm_x2[0, i] * norm_x1[1, i]
        A[i, 2] = norm_x2[0, i]
        A[i, 3] = norm_x2[1, i] * norm_x1[0, i]
        A[i, 4] = norm_x2[1, i] * norm_x1[1, i]
        A[i, 5] = norm_x2[1, i]
        A[i, 6] = norm_x1[0, i]
        A[i, 7] = norm_x1[1, i]
        A[i, 8] = 1.0

    # Solve for F using SVD
    # TODO
    U, S, V = numpy.linalg.svd(A)
    F = V.T[:, 8].reshape(3, 3)

    # Enforce that rank(F)=2
    # TODO
    U, S, V = np.linalg.svd(F)

    # Throw out smallest singular value (S contains singular values in descending order)
    S_prime = np.zeros((3, 3))
    S_prime[0, 0] = S[0]
    S_prime[1, 1] = S[1]
    F = np.matmul(np.matmul(U, S_prime), V)

    if normalize:
        # Transform F back
        # TODO
        F = np.matmul(np.matmul(Tx2.T, F), Tx1)

    return F


def right_epipole(F):
    """
    Computes the (right) epipole from a fundamental matrix F.
    (Use with F.T for left epipole.)
    """

    # The epipole is the null space of F (F * e = 0)
    # TODO
    e = null_space(F)
    e = e/e[2]

    return e


def plot_epipolar_line(im, F, x, e, plot = None):
    """
    Plot the epipole and epipolar line F*x=0 in an image. F is the fundamental matrix
    and x a point in the other image.
    """
    m, n = im.shape[:2]
    # TODO
    # plt.plot(100, 50, "ro")
    # plt.plot(e[0], e[1], "wo")

    line = np.dot(F, x)

    # epipolar line parameter and values
    t = np.linspace(0, n, 100)
    lt = np.array([(line[2] + line[0] * tt) / (-line[1]) for tt in t])

    # take only line points inside the image
    ndx = (lt >= 0) & (lt < m)

    if plot is None:
        plot = plt

    plot.plot(t[ndx], lt[ndx], linewidth=2)
