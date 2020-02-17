#include "Smoothing.hh"


// ======================================================================
// EXERCISE 1.1
// ========================================================================
void Smoothing::uniform_smooth(const int _iterations) {
    Point laplacian;
    int w = 0.;
    std::vector<Point> points_new(mesh_.n_vertices(), Point(0.));
    for (auto vh : mesh_.vertices())
        points_new[vh.idx()] = mesh_.point(vh);

    for (int iter=0; iter<_iterations; ++iter) {
        // ------------- IMPLEMENT HERE ---------

        // ------------- IMPLEMENT HERE ---------
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
    double ww = 0.;
    std::vector<Point> points_new(mesh_.n_vertices(), Point(0.));
    for (auto vh : mesh_.vertices())
        points_new[vh.idx()] = mesh_.point(vh);

    for (int iter=0; iter<_iterations; ++iter) {
        // update edge weights
        calc_edges_weights();
        // ------------- IMPLEMENT HERE ---------

        // ------------- IMPLEMENT HERE ---------
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
    calc_weights();
    // ------------- IMPLEMENT HERE ---------

    // ------------- IMPLEMENT HERE ---------
    mesh_.update_normals();
}

// ======================================================================
// EXERCISE 3.1
// ========================================================================
void Smoothing::uniform_laplacian_enhance_feature(const int _iterations, const int _coefficient) {
    // ------------- IMPLEMENT HERE ---------

    // ------------- IMPLEMENT HERE ---------
    mesh_.update_normals();
}

// ======================================================================
// EXERCISE 3.2
// ========================================================================
void Smoothing::cotan_laplacian_enhance_feature(const int _iterations, const int _coefficient) {
    // ------------- IMPLEMENT HERE ---------

    // ------------- IMPLEMENT HERE ---------
    mesh_.update_normals();
}