#include "Smoothing.hh"


// ======================================================================
// EXERCISE 1.1
// ========================================================================
void Smoothing::uniform_smooth(const int _iterations) {
    Point laplacian;
    int w;
    std::vector<Point> points_new(mesh_.n_vertices(), Point(0.));

    for (int iter=0; iter<_iterations; ++iter) {
        // compute new vertex positions by Laplacian smoothing
        for (auto vh : mesh_.vertices()) {
            laplacian = Point(0.);
            w = 0;

            if (!mesh_.is_boundary(vh)) {
                for(auto vv_it = mesh_.vv_iter(vh); vv_it.is_valid(); ++vv_it) {
                    laplacian += (mesh_.point(*vv_it) - mesh_.point(vh));
                    w++;
                }

                laplacian /= w;   // normalize by sum of weights
                laplacian *= 0.5;  // damping
            }

            points_new[vh.idx()] = mesh_.point(vh) + laplacian;
        }

        // update vertex positions
        for (auto vh : mesh_.vertices()) {
            mesh_.point(vh) = points_new[vh.idx()];
        }
    }

    mesh_.update_normals();
}

// ======================================================================
// EXERCISE 1.2
// ========================================================================
void Smoothing::cotan_laplacian_smooth(const int _iterations) {
    Point laplacian;
    double ww;
    std::vector<Point> points_new(mesh_.n_vertices(), Point(0.));

    for (int iter=0; iter<_iterations; ++iter) {
        // update edge weights
        calc_edges_weights();
        // compute new vertex positions by cotan Laplacian smoothing
        for (auto vh : mesh_.vertices()) {
            laplacian = Point(0.);
            ww = 0.;

            if (!mesh_.is_boundary(vh)) {
                for(auto voh_it = mesh_.voh_iter(vh); voh_it.is_valid(); ++voh_it) {
                    auto eh = mesh_.edge_handle(*voh_it);
                    auto vh_t = mesh_.to_vertex_handle(*voh_it);

                    laplacian += mesh_.property(edge_weight_, eh) * (mesh_.point(vh_t) - mesh_.point(vh));
                    ww += mesh_.property(edge_weight_, eh);
                }

                laplacian /= ww;   // normalize by sum of weights
                laplacian *= 0.5;  // damping
            }

            points_new[vh.idx()] = mesh_.point(vh) + laplacian;
        }

        // update vertex positions
        for (auto vh : mesh_.vertices()) {
            mesh_.point(vh) = points_new[vh.idx()];
        }
    }

    mesh_.update_normals();
}

// ======================================================================
// EXERCISE 2
// ========================================================================
void Smoothing::implicit_smooth(const double _timestep) {
    int n = (int)mesh_.n_vertices();
    // compute cotan edge weights and vertex areas
    calc_weights ();

    // A*X = B
    Eigen::SparseMatrix<double> A(n,n);
    Eigen::MatrixXd B(n,3);

    // nonzero elements of A as triplets: (row, column, value)
    std::vector< Eigen::Triplet<double> > triplets;

    // setup matrix A and rhs B
    for(auto vh : mesh_.vertices()) {
        // rhs row
        for (int dim = 0; dim < 3; ++dim) {
            B(vh.idx(), dim) = mesh_.point(vh)[dim] / mesh_.property(vertex_weight_, vh);
        }

        // lhs row
        double ww(0.0);
        for (auto voh_it = mesh_.voh_iter(vh); voh_it.is_valid(); ++voh_it) {
            auto eh = mesh_.edge_handle(*voh_it);
            auto vh_t = mesh_.to_vertex_handle(*voh_it);

            ww += mesh_.property(edge_weight_, eh);

            triplets.push_back(Eigen::Triplet<double>(vh.idx(), vh_t.idx(), -_timestep * mesh_.property(edge_weight_, eh)));
        }
        triplets.push_back(Eigen::Triplet<double>(vh.idx(), vh.idx(), 1.0 / mesh_.property(vertex_weight_, vh) + _timestep * ww));
    }

    // build sparse matrix from triplets
    A.setFromTriplets(triplets.begin(), triplets.end());

    // solve A*X = B
    Eigen::SimplicialLDLT< Eigen::SparseMatrix<double> > solver(A);
    if (solver.info() != Eigen::Success) {
        printf("linear solver init failed.\n");
        return;
    }
    Eigen::MatrixXd X = solver.solve(B);
    if (solver.info() != Eigen::Success) {
        printf("linear solver init failed.\n");
        return;
    }
    // copy solution
    Point pt(0.);
    for(auto vh : mesh_.vertices()) {
        for (int dim = 0; dim < 3; ++dim)
            pt[dim] = X(vh.idx(), dim);
        mesh_.set_point(vh, pt);
    }

    mesh_.update_normals();
}

// ======================================================================
// EXERCISE 3.1
// ========================================================================
void Smoothing::uniform_laplacian_enhance_feature(const int _iterations, const int _coefficient) {
    std::vector<Point> points_old(mesh_.n_vertices(), Point(0.));
    for(auto vh : mesh_.vertices()) {
        points_old[vh.idx()] = mesh_.point(vh);
    }

    uniform_smooth(_iterations);

    for(auto vh : mesh_.vertices())
        mesh_.point(vh) += (points_old[vh.idx()] - mesh_.point(vh)) * _coefficient;

    mesh_.update_normals();
}

// ======================================================================
// EXERCISE 3.2
// ========================================================================
void Smoothing::cotan_laplacian_enhance_feature(const int _iterations, const int _coefficient) {
    std::vector<Point> points_old(mesh_.n_vertices(), Point(0.));
    for(auto vh : mesh_.vertices()) {
        points_old[vh.idx()] = mesh_.point(vh);
    }

    cotan_laplacian_smooth(_iterations);

    for(auto vh : mesh_.vertices())
        mesh_.point(vh) += (points_old[vh.idx()] - mesh_.point(vh)) * _coefficient;

    mesh_.update_normals();
}