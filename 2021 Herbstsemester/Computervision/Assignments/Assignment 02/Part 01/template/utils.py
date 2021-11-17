import numpy as np
import matplotlib.pyplot as plt


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

    # Mean-Distances to Centroid
    mean_distance = np.sqrt(np.sum(np.square(np.apply_along_axis(lambda v: v - centroid, 0, x))) / (2*x.shape[1]))

    T[0, 0] = 1 / mean_distance
    T[0, 2] = - 1 / mean_distance * centroid[0]
    T[1, 1] = 1 / mean_distance
    T[1, 2] = - 1 / mean_distance * centroid[1]
    T[2, 2] = 1

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

    # Solve for F using SVD
    # TODO

    # Enforce that rank(F)=2
    # TODO

    if normalize:
        # Transform F back
        # TODO
        pass

    return F


def right_epipole(F):
    """
    Computes the (right) epipole from a fundamental matrix F.
    (Use with F.T for left epipole.)
    """

    # The epipole is the null space of F (F * e = 0)
    # TODO

    return e


def plot_epipolar_line(im, F, x, e):
    """
    Plot the epipole and epipolar line F*x=0 in an image. F is the fundamental matrix
    and x a point in the other image.
    """
    m, n = im.shape[:2]
    # TODO
