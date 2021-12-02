import numpy as np
import scipy.sparse as sparse
from scipy.sparse.linalg import lsmr


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

    s = np.sqrt(2) / np.mean(np.apply_along_axis(lambda v: np.linalg.norm(v - centroid), 0, x))

    # another approach to calculate s as found in the paper https://cs.adelaide.edu.au/~wojtek/papers/pami-nals2.pdf
    # s = 1 / np.sqrt(np.sum(np.square(np.apply_along_axis(lambda v: v - centroid, 0, x))) / (2 * x.shape[1]))

    T[0, 0] = s
    T[0, 2] = - s * centroid[0]
    T[1, 1] = s
    T[1, 2] = - s * centroid[1]
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

    if N < 8:  # not enough entries for eight-point-algorithm
        return

    if normalize:
        # Construct transformation matrices to normalize the coordinates
        # TODO
        Tx1 = get_normalization_matrix(x1)
        Tx2 = get_normalization_matrix(x2)

        # Normalize inputs
        # TODO
        x1 = np.apply_along_axis(lambda x: np.matmul(Tx1, x), 0, x1)
        x2 = np.apply_along_axis(lambda x: np.matmul(Tx2, x), 0, x2)

    # Construct matrix A encoding the constraints on x1 and x2
    # TODO

    A = np.zeros((x1.shape[1], 9))
    for i in range(x1.shape[1]):
        A[i, 0] = x2[0, i] * x1[0, i]
        A[i, 1] = x2[0, i] * x1[1, i]
        A[i, 2] = x2[0, i]
        A[i, 3] = x2[1, i] * x1[0, i]
        A[i, 4] = x2[1, i] * x1[1, i]
        A[i, 5] = x2[1, i]
        A[i, 6] = x1[0, i]
        A[i, 7] = x1[1, i]
        A[i, 8] = 1.0

    # Solve for F using SVD
    # TODO
    U, S, V = np.linalg.svd(A)
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


def ransac(x1, x2, threshold, num_steps=1000, random_seed=42):
    if random_seed is not None:
        np.random.seed(random_seed)  # we are using a random seed to make the results reproducible

    # TODO setup variables
    most_inliers = None
    optimal_count = 0.0

    # TODO calculate initial inliers with with the best candidate points
    for _ in range(num_steps):
        selected_points = np.random.choice(x1.shape[1], 8, replace=False)
        helper_fundamental_matrix = eight_points_algorithm(x1[:, selected_points], x2[:, selected_points])
        squared_error = np.square(np.sum(x2 * (helper_fundamental_matrix @ x1), axis=0))
        help_inliers = squared_error < threshold
        count = help_inliers.sum()
        if count > optimal_count:
            most_inliers = help_inliers
            optimal_count = count

    # TODO estimate F with all the inliers
    # TODO find final inliers with F
    while True:
        F = eight_points_algorithm(x1[:, most_inliers], x2[:, most_inliers])
        squared_error = np.square(np.sum(x2 * (F @ x1), axis=0))
        inliers = squared_error < threshold
        if (inliers == most_inliers).all():
            return F, inliers # F is estimated fundamental matrix and inliers is an indicator (boolean) numpy array
        most_inliers = inliers


def decompose_essential_matrix(E, x1, x2):
    """
    Decomposes E into a rotation and translation matrix using the
    normalized corresponding points x1 and x2.
    """

    # Fix left camera-matrix
    Rl = np.eye(3)
    tl = np.array([[0, 0, 0]]).T
    Pl = np.concatenate((Rl, tl), axis=1)

    # TODO: Compute possible rotations and translations
    # from https://stackoverflow.com/questions/22807039/decomposition-of-essential-matrix-validation-of-the-four-possible-solutions-for/22808118#22808118
    U, S, V = np.linalg.svd(E)
    if np.linalg.det(U) < 0:
        U = -U
    if np.linalg.det(V) < 0:
        V = -V

    W = np.array([[0, -1, 0],
                  [1, 0, 0],
                  [0, 0, 1]])
    R1 = U @ W.T @ V
    R2 = U @ W @ V

    t1 = U[:, 2].reshape(3, 1)
    t2 = -t1

    # Four possibilities
    Pr = [np.concatenate((R1, t1), axis=1),
          np.concatenate((R1, t2), axis=1),
          np.concatenate((R2, t1), axis=1),
          np.concatenate((R2, t2), axis=1)]

    # Compute reconstructions for all possible right camera-matrices
    X3Ds = [infer_3d(x1[:, 0:1], x2[:, 0:1], Pl, x) for x in Pr]

    # Compute projections on image-planes and find when both cameras see point
    test = [np.prod(np.hstack((Pl @ np.vstack((X3Ds[i], [[1]])), Pr[i] @ np.vstack((X3Ds[i], [[1]])))) > 0, 1) for i in
            range(4)]
    test = np.array(test)
    idx = np.where(np.hstack((test[0, 2], test[1, 2], test[2, 2], test[3, 2])) > 0.)[0][0]

    # Choose correct matrix
    Pr = Pr[idx]

    return Pl, Pr


def infer_3d(x1, x2, Pl, Pr):
    # INFER3D Infers 3d-positions of the point-correspondences x1 and x2, using
    # the rotation matrices Rl, Rr and translation vectors tl, tr. Using a
    # least-squares approach.

    M = x1.shape[1]
    # Extract rotation and translation
    Rl = Pl[:3, :3]
    tl = Pl[:3, 3]
    Rr = Pr[:3, :3]
    tr = Pr[:3, 3]

    # Construct matrix A with constraints on 3d points
    row_idx = np.tile(np.arange(4 * M), (3, 1)).T.reshape(-1)
    col_idx = np.tile(np.arange(3 * M), (1, 4)).reshape(-1)

    A = np.zeros((4 * M, 3))
    A[:M, :3] = x1[0:1, :].T @ Rl[2:3, :] - np.tile(Rl[0:1, :], (M, 1))
    A[M:2 * M, :3] = x1[1:2, :].T @ Rl[2:3, :] - np.tile(Rl[1:2, :], (M, 1))
    A[2 * M:3 * M, :3] = x2[0:1, :].T @ Rr[2:3, :] - np.tile(Rr[0:1, :], (M, 1))
    A[3 * M:4 * M, :3] = x2[1:2, :].T @ Rr[2:3, :] - np.tile(Rr[1:2, :], (M, 1))

    A = sparse.csr_matrix((A.reshape(-1), (row_idx, col_idx)), shape=(4 * M, 3 * M))

    # Construct vector b
    b = np.zeros((4 * M, 1))
    b[:M] = np.tile(tl[0], (M, 1)) - x1[0:1, :].T * tl[2]
    b[M:2 * M] = np.tile(tl[1], (M, 1)) - x1[1:2, :].T * tl[2]
    b[2 * M:3 * M] = np.tile(tr[0], (M, 1)) - x2[0:1, :].T * tr[2]
    b[3 * M:4 * M] = np.tile(tr[1], (M, 1)) - x2[1:2, :].T * tr[2]

    # Solve for 3d-positions in a least-squares way
    w = lsmr(A, b)[0]
    x3d = w.reshape(M, 3).T

    return x3d
