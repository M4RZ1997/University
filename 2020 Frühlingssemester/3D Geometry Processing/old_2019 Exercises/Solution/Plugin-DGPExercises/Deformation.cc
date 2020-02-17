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
    for(auto vh : mesh_.vertices()) {
        double cotan_sum = 0.0;
        for (auto voh_it = mesh_.voh_iter(vh); voh_it.is_valid(); ++voh_it) {
            auto vh_t = mesh_.to_vertex_handle(*voh_it);
            auto eh = mesh_.edge_handle(*voh_it);
            cotan_sum += mesh_.property(edge_weight_, eh);
            triplets_L.push_back(Eigen::Triplet<double>(vh.idx(), vh_t.idx(), mesh_.property(edge_weight_, eh)));
        }
        triplets_L.push_back(Eigen::Triplet<double>(vh.idx(), vh.idx(), -cotan_sum));
    }

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

    L.setFromTriplets(triplets_L.begin(), triplets_L.end());
    Eigen::SparseMatrix<double> L2 = L.transpose()*L;// or L;

        for (size_t j = 0; j < _fixed_vts.size(); j++) {
                for (Eigen::SparseMatrix<double>::InnerIterator it(L2, _fixed_vts[j]); it; ++it)
                    it.valueRef() = 0;
                L2.coeffRef(_fixed_vts[j], _fixed_vts[j]) = 1;
                rhs(_fixed_vts[j], 0) = 0;
            }

        for (size_t j = 0; j < _displaced_vts.size(); j++) {
                for (Eigen::SparseMatrix<double>::InnerIterator it(L2, _displaced_vts[j]); it; ++it)
                    it.valueRef() = 0;
                L2.coeffRef(_displaced_vts[j], _displaced_vts[j]) = 1;

                rhs(_displaced_vts[j], 0) = _displacement[0];
                rhs(_displaced_vts[j], 1) = _displacement[1];
                rhs(_displaced_vts[j], 2) = _displacement[2];
            }

    Eigen::SparseMatrix<double> L2T = L2.transpose();


    Eigen::SparseLU< Eigen::SparseMatrix<double> > solver(L2T);
    if (solver.info() != Eigen::Success) {
        printf("linear solver init failed.\n");
    }
    Eigen::MatrixXd X = solver.solve(rhs);
    if (solver.info() != Eigen::Success) {
        printf("linear solver failed.\n");
    }

    // copy solution
    Point pt(0.);
    for(auto vh : mesh_.vertices()) {
        for (int dim = 0; dim < 3; ++dim)
            pt[dim] = X(vh.idx(), dim);
        mesh_.point(vh) += pt;
    }

    mesh_.update_normals();

}