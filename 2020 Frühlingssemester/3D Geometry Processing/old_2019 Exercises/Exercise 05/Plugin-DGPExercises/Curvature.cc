#include <math.h>
#include <algorithm>
#include "Curvature.hh"

void Curvature::show_normal(const int _normal_type) {
    switch(_normal_type) {
        case 0: compute_normals_with_constant_weights();
            break;
        case 1: compute_normals_by_area_weights();
            break;
        default: compute_normals_with_angle_weights();
    }
}

void Curvature::show_curvature(const int _curvature_type) {
    switch(_curvature_type) {
        case 0: calc_uniform_laplacian();
            break;
        case 1: calc_mean_curvature();
            break;
        default: calc_gauss_curvature();
    }
}

//====================================================================================================================//
// ========================================================================
// EXERCISE 1.1
// ========================================================================
void Curvature::compute_normals_with_constant_weights() {
    // ------------- IMPLEMENT HERE ---------
    // Compute the normals for each vertex v in the mesh using the constant weights
    // technique (see handout) and store it inside vertex_normal_


    // ------------- IMPLEMENT HERE ---------
    update_normals();
}
// ========================================================================
// EXERCISE 1.2
// ========================================================================
void Curvature::compute_normals_by_area_weights() {
    // ------------- IMPLEMENT HERE ---------
    // Compute the normals for each vertex v in the mesh using the weights proportionals
    // to the areas technique (see handout) and store inside vertex_normal_


    // ------------- IMPLEMENT HERE ---------
    update_normals();
}
// ========================================================================
// EXERCISE 1.3
// ========================================================================
void Curvature::compute_normals_with_angle_weights() {
    // ------------- IMPLEMENT HERE ---------
    // Compute the normals for each vertex v in the mesh using the weights proportionals
    // to the angles technique (see handout) and store it inside vertex_normal_


    // ------------- IMPLEMENT HERE ---------
    update_normals();
}
// ========================================================================
// EXERCISE 2.1
// ========================================================================
void Curvature::calc_uniform_laplacian() {
    min_curvature_ = DBL_MAX;
    max_curvature_ = DBL_MIN;
    // ------------- IMPLEMENT HERE ---------
    // For each non-boundary vertex, compute uniform Laplacian operator vector
    // and store the vector length in the vertex property of the
    // mesh called vertex_curvature_.
    // Store min and max values of vertex_curvature_ in min_curvature_ and max_curvature_.


    // ------------- IMPLEMENT HERE ---------
    color_coding();

    std::cout<<"Min Uniform Laplace value is: " << min_curvature_ << std::endl;
    std::cout<<"Max Uniform Laplace value is: " << max_curvature_ << std::endl;
}
// ========================================================================
// EXERCISE 2.2
// ========================================================================
void Curvature::calc_mean_curvature() {
    min_curvature_ = DBL_MAX;
    max_curvature_ = DBL_MIN;

    calc_weights();
    // ------------- IMPLEMENT HERE ---------
    // For all non-boundary vertices, approximate the mean curvature using
    // the length of the Laplace-Beltrami approximation.
    // Save your approximation in vertex_curvature_ vertex property of the mesh.
    // Use the weights from calc_weights(): e_weight and v_weight


    // ------------- IMPLEMENT HERE ---------
    color_coding();

    std::cout<<"Min Laplace-Beltrami Curvature value is: " << min_curvature_ << std::endl;
    std::cout<<"Max Laplace-Beltrami Curvature value is: " << max_curvature_ << std::endl;
}
// ========================================================================
// EXERCISE 2.3
// ========================================================================
void Curvature::calc_gauss_curvature() {
    min_curvature_ = DBL_MAX;
    max_curvature_ = DBL_MIN;

    calc_weights();
    // ------------- IMPLEMENT HERE ---------
    // For each non-boundary vertex, approximate Gaussian curvature,
    // and store it in the vertex property v_gauss_curvature.
    // Hint: When calculating angles out of cross products make sure the value
    // you pass to the acos function is between -1.0 and 1.0.
    // Use the vertex_weight_ property for the area weight.


    // ------------- IMPLEMENT HERE ---------
    color_coding();

    std::cout<<"Min Gauss Curvature value is: " << min_curvature_ << std::endl;
    std::cout<<"Max Gauss Curvature value is: " << max_curvature_ << std::endl;
}

//====================================================================================================================//
void Curvature::calc_weights() {
    calc_vertices_weights();
    calc_edges_weights();
}

void Curvature::calc_vertices_weights() {
    double area = 0.;
    for (auto vh: mesh_.vertices()) {
        area = 0.0;

        for(auto vih_it = mesh_.vih_iter(vh); vih_it.is_valid(); ++vih_it) {
            if(mesh_.is_boundary(*vih_it))
                continue;

            area += mesh_.calc_sector_area(*vih_it) * 0.3333f;
        }

        mesh_.property(vertex_weight_, vh) = 0.5 / area;
    }
}

void Curvature::calc_edges_weights() {
    OpenMesh::HalfedgeHandle heh0, heh1, heh2;
    OpenMesh::VertexHandle vh0, vh1;
    Point p0(0,0,0), p1(0,0,0), p2(0,0,0), d0(0,0,0), d1(0,0,0);
    double dot, cross;
    double w = 0.;
    for (auto eh: mesh_.edges()) {
        if(mesh_.is_boundary(eh))
            continue;

        w = 0.0;

        heh0 = mesh_.halfedge_handle(eh, 0);
        vh0 = mesh_.to_vertex_handle(heh0);
        p0 = mesh_.point(vh0);

        heh1 = mesh_.halfedge_handle(eh, 1);
        vh1 = mesh_.to_vertex_handle(heh1);
        p1 = mesh_.point(vh1);

        heh2 = mesh_.next_halfedge_handle(heh0);
        p2 = mesh_.point(mesh_.to_vertex_handle(heh2));
        d0 = (p0 - p2).normalize();
        d1 = (p1 - p2).normalize();
        dot = std::min(1.0, std::max(-1.0, d0 | d1));
        cross = std::min(1.0, std::max(1e-16, (d0 % d1).norm()));
        w += dot / cross;

        heh2 = mesh_.next_halfedge_handle(heh1);
        p2 = mesh_.point(mesh_.to_vertex_handle(heh2));
        d0 = (p0 - p2).normalize();
        d1 = (p1 - p2).normalize();
        dot = std::min(1.0, std::max(-1.0, d0 | d1));
        cross = std::min(1.0, std::max(1e-16, (d0 % d1).norm()));
        w += dot / cross;

        w = std::max(0., w);
        mesh_.property(edge_weight_, eh) = w * 0.5;
    }
}

void Curvature::update_normals() {
    for(auto vh : mesh_.vertices())
        mesh_.set_normal(vh, 0.5*avr_e_length_*mesh_.property(vertex_normal_, vh));
}

void Curvature::color_coding(const int _bound) {
    auto values = mesh_.property(vertex_curvature_).data_vector();
    // discard upper and lower bound
    auto n = values.size()-1;
    auto i = n / _bound;
    std::sort(values.begin(), values.end());
    auto min_value = values[i], max_value = values[n-1-i];
    const auto range = max_value - min_value;
    color_.set_range(0, 1.0, false);

    for(auto vh : mesh_.vertices()) {
        auto t = (mesh_.property(vertex_curvature_, vh) - min_value)/range;
        mesh_.set_color(vh, color_.color_float4(t));
    }
}