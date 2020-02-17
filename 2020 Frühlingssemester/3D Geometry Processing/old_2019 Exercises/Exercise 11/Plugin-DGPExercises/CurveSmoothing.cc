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
// Exercise 4 : fill the 2 functions below (see PDF for instructions)
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
        for(unsigned int i=0; i<num_vertices; ++i) {
            auto pt = (1 - _epsilon) * points_old[i] + _epsilon * (points_old[(i + num_vertices -1) % num_vertices] +
                    points_old[(i + 1) % num_vertices]) / 2.;

            points_new[i] = pt;
        }

        //compute the curve lengths
        double l_old = 0.;
        double l_new = 0.;

        for(unsigned int i=0; i<num_vertices; ++i) {
            l_old += (points_old[i] - points_old[(i + 1) % num_vertices]).norm();
            l_new += (points_new[i] - points_new[(i + 1) % num_vertices]).norm();
        }

        double scale = l_old / l_new;
        Point center(0., 0., 0.);
        for(auto pt : points_new)
            center += pt;
        center /= static_cast<double>(num_vertices);

        for(auto& pt : points_new)
            pt = (pt - center) * scale + center;
        //--------------------------------------

        points_old = points_new;
    }

    if(!points_new.empty()){
        int j=0;
        auto v_it = mesh_.v_iter();
        for(v_it, j; v_it.valid(); ++v_it, ++j)
            mesh_.set_vertex(*v_it, points_new[j]);
    }
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
        for(unsigned int i=0; i<num_vertices; ++i) {
            const Point& p0 = points_old[(i + num_vertices -1) % num_vertices];
            const Point& p1 = points_old[i];
            const Point& p2 = points_old[(i + 1) % num_vertices];

            //find the center of the circumcircle
            Point center(0., 0., 0.);

            //works for 3D
//            auto v20 = p0 - p2;
//            auto v21 = p1 - p2;
//            auto cross = v20 % v21;
//
//            auto upper = ((v20|v20) * v21 - (v21|v21) * v20) % cross;
//            auto val2 = val1 / (2* (cross | cross));
//
//            center = val2 + p2;

            double dA = p0[0]*p0[0] + p0[1]*p0[1];
            double dB = p1[0]*p1[0] + p1[1]*p1[1];
            double dC = p2[0]*p2[0] + p2[1]*p2[1];

            double aux1 = (dA*(p2[1] - p1[1]) + dB*(p0[1] - p2[1]) + dC*(p1[1] - p0[1]));
            double aux2 = -(dA*(p2[0] - p1[0]) + dB*(p0[0] - p2[0]) + dC*(p1[0] - p0[0]));
            double div = 2*(p0[0]*(p2[1] - p1[1]) + p1[0]*(p0[1]-p2[1]) + p2[0]*(p1[1] - p0[1]));

            if(div != 0) {
                center[0] = aux1 / div;
                center[1] = aux2 / div;
            }

            auto v1c = center - p1;
            auto pt = p1 + _epsilon * v1c / (v1c | v1c);

            points_new[i] = pt;
        }

        //compute the curve lengths
        double l_old = 0.;
        double l_new = 0.;

        for(unsigned int i=0; i<num_vertices; ++i) {
            l_old += (points_old[i] - points_old[(i + 1) % num_vertices]).norm();
            l_new += (points_new[i] - points_new[(i + 1) % num_vertices]).norm();
        }

        double scale = l_old / l_new;
        Point center1(0., 0., 0.);
        for(auto pt : points_new)
            center1 += pt;
        center1 /= static_cast<double>(num_vertices);

        for(auto& pt : points_new)
            pt = (pt - center1) * scale + center1;

        //--------------------------------------

        points_old = points_new;
    }

    if(!points_new.empty()){
        int j=0;
        auto v_it = mesh_.v_iter();
        for(v_it, j; v_it.valid(); ++v_it, ++j)
            mesh_.set_vertex(*v_it, points_new[j]);
    }
}

