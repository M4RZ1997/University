#include <math.h>
#include <random>
#include "CurveSmoothing.hh"

void
CurveSmoothing::
generate_curve(const int _num_vertices) {
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0., 5*3e-2);

    Point center(0., 0., 0.);
    double radius = 0.3;

    std::vector<OpenVolumeMesh::VertexHandle> vhs;
    Point pt;
    for(int i=0; i<_num_vertices; ++i) {
        double frac = static_cast<double>(i) / static_cast<double>(_num_vertices);
        pt[0] = center[0] + radius * cos(2. * M_PI * frac) + distribution(generator);
        pt[1] = center[0] + radius * sin(2. * M_PI * frac) + distribution(generator);

        vhs.push_back(mesh_.add_vertex(pt));
    }

    for(int i=0 ;i<_num_vertices; ++i)
        mesh_.add_edge(vhs[i], vhs[(i+1) % _num_vertices]);
}

// ============================================================================
// Exercise 4 : fill the two functions below (see PDF for instructions)
// Store the smoothed curve points in vector points_new
// Hint : try to play with epsilon
// ============================================================================

void
CurveSmoothing::laplacian_smoothing(const int _num_iter, const double _epsilon) {
    unsigned int num_vertices = mesh_.n_vertices();

    std::vector<Point> points_new(num_vertices, Point(0., 0., 0.));

    std::vector<Point> points_old;
    points_old.reserve(num_vertices);
    for(auto vh : mesh_.vertices())
        points_old.push_back(mesh_.vertex(vh));

    for(int iter=0; iter<_num_iter; ++iter) {
        //----------Add your code here----------

        //--------------------------------------

        points_old = points_new;
    }

    int j=0;
    auto v_it = mesh_.v_iter();
    for(v_it, j; v_it.valid(); ++v_it, ++j)
        mesh_.set_vertex(*v_it, points_new[j]);
}

void
CurveSmoothing::osculating_circle(const int _num_iter, const double _epsilon){
    unsigned int num_vertices = mesh_.n_vertices();

    std::vector<Point> points_new(num_vertices, Point(0., 0., 0.));

    std::vector<Point> points_old;
    points_old.reserve(num_vertices);
    for(auto vh : mesh_.vertices())
        points_old.push_back(mesh_.vertex(vh));

    for(int iter=0; iter<_num_iter; ++iter) {
        //----------Add your code here----------

        //--------------------------------------

        points_old = points_new;
    }

    int j=0;
    auto v_it = mesh_.v_iter();
    for(v_it, j; v_it.valid(); ++v_it, ++j)
        mesh_.set_vertex(*v_it, points_new[j]);
}

