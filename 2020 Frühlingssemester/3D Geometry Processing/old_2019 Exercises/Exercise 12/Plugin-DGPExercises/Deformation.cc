#include "Deformation.hh"

void Deformation::deform(const std::vector<int>& _fixed_vts, const std::vector<int>& _displaced_vts,
                         const Point& _displacement) {
    const int N = mesh_.n_vertices();

    calc_edges_weights();

    Eigen::SparseMatrix<double> L(N, N);
    Eigen::MatrixXd rhs(Eigen::MatrixXd::Zero(N, 3));
    std::vector< Eigen::Triplet<double> > triplets_L;

    // ------------- IMPLEMENT HERE ---------
    // Set up Laplace-Beltrami matrix of the mesh



    // ------------- IMPLEMENT HERE ---------

    // ------------- IMPLEMENT HERE ---------
    // Compute squared Laplace-Beltrami matrix
    // For each fixed and shifted vertex replace the corresponding row of squared Laplace-Beltrami matrix with the constraint row
    //    Hint: to iterate through sparse matrix use Eigen::SparseMatrix<double>::InnerIterator.
    //          note that sparse matrix is stored column-wise
    //          since squared Laplace-Beltrami matrix is symmetric,
    //          you can traverse the matrix column-wise and transpose the result
    // Solve the linear system L2x = b
    // Displace the vertices by x



    // ------------- IMPLEMENT HERE ---------

    mesh_.update_normals();

}