import numpy as np
from scipy import sparse


def hessian_matrix(u, reg, k_type):
    r"""
    This function implements the Hessian of the follwoing tv and corresponding data term.
    The formula of TV:
    $   || \nabla u||_{2}  \cong \sum_{i=0}^{m-2} \sum_{j=0}^{n-2}
            (u[i+1,j]- u[i,j])^2 + (u[i,j+1]- u[i,j])^2) +
            \sum_{j=0}^{n-2} (u[m-1, j+1] - u[m-1, j])^2 +
            \sum_{i=0}^{m-2} (u[i+1,n-1] - u[i, n-1])^2
    $
    """

    # let 'u' be the known image
    m, n, *_ = u.shape

    # define the coordinates of a neighborhood around each pixel
    # (center, north, south, west, east, north-west, north-east, south-west, south-east)
    # there are 9 possible neighbors
    rows = np.zeros((9, m * n))
    cols = np.zeros((9, m * n))
    vals = np.zeros((9, m * n))

    for i in range(n):
        for j in range(m):

            c_tv_1 = (i > 0 and i < n - 1 and j > 0 and j < m - 1) and True
            c_tv_2 = (i == 0 and j == 0) and True
            c_tv_3 = (i > 0 and i < n - 1 and j == 0) and True
            c_tv_4 = (i >= 0 and i < n - 1 and j == m - 1) and True
            c_tv_5 = (i == n - 1 and j >= 0 and j < m - 1) and True
            c_tv_6 = (i == 0 and j >= 1 and j < m - 1) and True

            c_tv_7 = (i == n - 1 and j > 0 and j < m - 1) and True
            c_tv_8 = (i == n - 1 and j == 0) and True
            c_tv_9 = (i == n - 1 and j == m - 1) and True

            c_tv_10 = (i > 0 and i < n - 1 and j == m - 1) and True
            c_tv_11 = (i == 0 and j == m - 1) and True
            c_tv_12 = (i == n - 1 and j == m - 1) and True

            # center pixel (i, j)
            val_center = 8 * reg * c_tv_1 + 4 * reg * c_tv_2 + 6 * reg * c_tv_3 + 2 * reg * c_tv_4 + 2 * reg * c_tv_5 + 6 * reg * c_tv_6 + 4 * reg * c_tv_7 + 2 * reg * c_tv_8 + 2 * reg * c_tv_9 + 4 * reg * c_tv_10 + 2 * reg * c_tv_11 + 2 * reg * c_tv_12
            # northern pixel (i, j - 1)
            val_northern = -2 * reg * c_tv_1 - 2 * reg * c_tv_4 - 2 * reg * c_tv_6 - 2 * reg * c_tv_7 - 2 * reg * c_tv_9
            # southern pixel (i, j + 1)
            val_southern = -2 * reg * c_tv_1 - 2 * reg * c_tv_2 - 2 * reg * c_tv_3 - 2 * reg * c_tv_6 - 2 * reg * c_tv_7 - 2 * reg * c_tv_8
            # western pixel  (i - 1, j)
            val_western = -2 * reg * c_tv_1 - 2 * reg * c_tv_3 - 2 * reg * c_tv_5 - 2 * reg * c_tv_10 - 2 * reg * c_tv_12
            # eastern pixel  (i + 1, j)
            val_eastern = -2 * reg * c_tv_1 - 2 * reg * c_tv_2 - 2 * reg * c_tv_3 - 2 * reg * c_tv_6 - 2 * reg * c_tv_10 - 2 * reg * c_tv_11
            # north-western pixel (i - 1, j - 1)
            val_north_western = 0
            # north-eastern pixel (i + 1, j - 1)
            val_north_eastern = 0
            # south-western pixel (i - 1, j + 1)
            val_south_western = 0
            # south-eastern pixel  (i + 1, j + 1)
            val_south_eastern = 0

            if k_type == 0:
                c_dt_1 = (i > 0 and i < n and j > 0 and j < m - 1) and True
                c_dt_2 = (i > 0 and j == 0) and True
                c_dt_3 = (i > 0 and j == m - 1) and True
                # center pixel (i, j)
                val_center += 1 * c_dt_1 + 0.5 * c_dt_2 + 0.5 * c_dt_3
                # northern pixel (i, j - 1)
                val_northern += 0.5 * c_dt_1 + +  0.5 * c_dt_3
                # southern pixel (i, j + 1)
                val_southern += 0.5 * c_dt_1 + 0.5 * c_dt_2
                # western pixel  (i - 1, j)
                val_western += 0
                # eastern pixel  (i + 1, j)
                val_eastern += 0
                # north-western pixel (i - 1, j - 1)
                val_north_western += 0
                # north-eastern pixel (i + 1, j - 1)
                val_north_eastern += 0
                # south-western pixel (i - 1, j + 1)
                val_south_western += 0
                # south-eastern pixel  (i + 1, j + 1)
                val_south_eastern += 0

            elif k_type == 1:
                c_dt_1 = (i > 0 and i < n - 1 and j > 0)
                c_dt_2 = (i == 0 and j > 0)
                c_dt_3 = (i == n - 1 and j > 0)
                # center pixel (i, j)
                val_center += 1 * c_dt_1 + 0.5 * c_dt_2 + 0.5 * c_dt_3
                # northern pixel (i, j - 1)
                val_northern += 0
                # southern pixel (i, j + 1)
                val_southern += 0
                # western pixel  (i - 1, j)
                val_western += 0.5 * c_dt_1 + 0.5 * c_dt_3
                # eastern pixel  (i + 1, j)
                val_eastern += 0.5 * c_dt_1 + 0.5 * c_dt_2
                # north-western pixel (i - 1, j - 1)
                val_north_western += 0
                # north-eastern pixel (i + 1, j - 1)
                val_north_eastern += 0
                # south-western pixel (i - 1, j + 1)
                val_south_western += 0
                # south-eastern pixel  (i + 1, j + 1)
                val_south_eastern += 0

            elif k_type == 2:
                c_dt_1 = (i > 0 and i < n - 1 and j > 0 and j < m - 1)
                c_dt_2 = (i < n - 1 and j == 0)
                c_dt_3 = (i == 0 and j > 0 and j < m - 1)
                c_dt_4 = (i > 0 and i < n - 1 and j == m - 1)
                c_dt_5 = (i == n - 1 and j > 0)
                # center pixel (i, j)
                val_center += 1 * c_dt_1 + 0.5 * c_dt_2 + 0.5 * c_dt_3 + 0.5 * c_dt_4 + 0.5 * c_dt_5
                # northern pixel (i, j - 1)
                val_northern += 0
                # southern pixel (i, j + 1)
                val_southern += 0
                # western pixel  (i - 1, j)
                val_western += 0
                # eastern pixel  (i + 1, j)
                val_eastern += 0
                # north-western pixel (i - 1, j - 1)
                val_north_western += 0.5 * c_dt_1 + 0.5 * c_dt_4 + 0.5 * c_dt_5
                # north-eastern pixel (i + 1, j - 1)
                val_north_eastern += 0
                # south-western pixel (i - 1, j + 1)
                val_south_western += 0
                # south-eastern pixel  (i + 1, j + 1)
                val_south_eastern += 0.5 * c_dt_1 + 0.5 * c_dt_2 + 0.5 * c_dt_3

            elif k_type == 3:
                c_dt_1 = (i > 0 and i < n - 1 and j > 0 and j < m - 1) and True
                c_dt_2 = (i > 0 and j == 0) and True
                c_dt_3 = (i < n - 1 and j == m - 1) and True
                c_dt_4 = (i == 0 and j > 0 and j < m - 1) and True
                c_dt_5 = (i == n - 1 and j > 0 and j < m - 1) and True

                # center pixel (i, j)
                val_center += 1 * c_dt_1 + 0.5 * c_dt_2 + 0.5 * c_dt_3 + 0.5 * c_dt_4 + 0.5 * c_dt_5
                # northern pixel (i, j - 1)
                val_northern += 0
                # southern pixel (i, j + 1)
                val_southern += 0
                # western pixel  (i - 1, j)
                val_western += 0
                # eastern pixel  (i + 1, j)
                val_eastern += 0
                # north-western pixel (i - 1, j - 1)
                val_north_western += 0
                # north-eastern pixel (i + 1, j - 1)
                val_north_eastern += 0.5 * c_dt_1 + 0.5 * c_dt_3 + 0.5 * c_dt_4
                # south-western pixel (i - 1, j + 1)
                val_south_western += 0.5 * c_dt_1 + 0.5 * c_dt_2 + 0.5 * c_dt_5
                # south-eastern pixel  (i + 1, j + 1)
                val_south_eastern += 0

            # center pixel (i, j)
            rows[0, i * m + j] = i * m + j
            cols[0, i * m + j] = i * m + j
            vals[0, i * m + j] += val_center
            # northern pixel (i, j - 1)
            if j > 0:
                rows[1, i * m + j] = i * m + j
                cols[1, i * m + j] = i * m + j - 1
                vals[1, i * m + j] = val_northern

            if j < m - 1:
                # southern pixel (i, j + 1)
                rows[2, i * m + j] = i * m + j
                cols[2, i * m + j] = i * m + j + 1
                vals[2, i * m + j] = val_southern

            if i > 0:
                # western pixel  (i - 1, j)
                rows[3, i * m + j] = i * m + j
                cols[3, i * m + j] = (i - 1) * m + j
                vals[3, i * m + j] = val_western

            if i < n - 1:
                # eastern pixel  (i + 1, j)
                rows[4, i * m + j] = i * m + j
                cols[4, i * m + j] = (i + 1) * m + j
                vals[4, i * m + j] = val_eastern

            if i > 0 and j > 0:
                # north-western pixel (i - 1, j - 1)
                rows[5, i * m + j] = i * m + j
                cols[5, i * m + j] = (i - 1) * m + j - 1
                vals[5, i * m + j] = val_north_western

            if i < n - 1 and j <= m - 1:  # Llukman: added "="
                # north-eastern pixel (i + 1, j - 1)
                rows[6, i * m + j] = i * m + j
                cols[6, i * m + j] = (i + 1) * m + j - 1
                vals[6, i * m + j] = val_north_eastern

            if i > 0 and j < m - 1:
                # south-western pixel (i - 1, j + 1)
                rows[7, i * m + j] = i * m + j
                cols[7, i * m + j] = (i - 1) * m + j + 1
                vals[7, i * m + j] = val_south_western

            if i < n - 1 and j < n - 1:
                # south-eastern pixel  (i + 1, j + 1)
                rows[8, i * m + j] = i * m + j
                cols[8, i * m + j] = (i + 1) * m + j + 1
                vals[8, i * m + j] = val_south_eastern

    # generate a sparse matrix
    rows = rows.reshape(-1)
    cols = cols.reshape(-1)
    vals = vals.reshape(-1)
    #    for i in range(rows.size):
    #        if vals[i] != 0:
    #            print(rows[i], cols[i], vals[i])
    A = sparse.csr_matrix((vals, (rows, cols)), shape=(n * m, n * m))
    return A
