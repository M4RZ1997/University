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

//    auto values = mesh_.property(vertex_curvature_).data_vector();
    color_coding(vertex_curvature_);
}

void Curvature::show_valence() {
    for(auto vh : mesh_.vertices())
        mesh_.property(vertex_valence_, vh) = mesh_.valence(vh);

//    auto values = mesh_.property(vertex_valence_).data_vector();
    color_coding(vertex_valence_, 3, 8);
}

//====================================================================================================================//
// ========================================================================
// EXERCISE 1.1
// ========================================================================
void Curvature::compute_normals_with_constant_weights() {
    // ------------- IMPLEMENT HERE ---------
    // Compute the normals for each vertex v in the mesh using the constant weights
    // technique (see handout) and store it inside vertex_normal_
    mesh_.update_face_normals();
    // for each vertex, we compute average of all the face normals around it
    for (auto vh: mesh_.vertices()) {
        Point n(0,0,0);
        for (auto vf_it = mesh_.vf_iter(vh); vf_it.is_valid(); ++vf_it) {
            n += mesh_.normal(*vf_it);
        }
        if(n.norm() > std::numeric_limits<double>::min())
            n.normalize();
        mesh_.property(vertex_normal_, vh) = n;
    }
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
    for (auto vh: mesh_.vertices()) {
        Point nn(0,0,0);
        auto p0 = mesh_.point(vh);
        for(auto voh_it = mesh_.voh_ccwbegin(vh); voh_it.is_valid(); ++voh_it) {
            if(mesh_.is_boundary(*voh_it))
                continue;

            auto p1 = mesh_.point(mesh_.to_vertex_handle(*voh_it));
            auto heh_pre = mesh_.prev_halfedge_handle(*voh_it);
            auto p2 = mesh_.point(mesh_.from_vertex_handle(heh_pre));
            p1 -= p0;
            p2 -= p0;

            nn += p1%p2;
        }
        if(nn.norm() > std::numeric_limits<double>::min())
            nn.normalize();
        mesh_.property(vertex_normal_, vh) = nn;
    }
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
    for (auto vh: mesh_.vertices()) {
        Point nn(0,0,0);
        auto p0 = mesh_.point(vh);
        for(auto voh_it = mesh_.voh_ccwbegin(vh); voh_it.is_valid(); ++voh_it) {
            if(mesh_.is_boundary(*voh_it))
                continue;

            auto p1 = mesh_.point(mesh_.to_vertex_handle(*voh_it));
            auto heh_pre = mesh_.prev_halfedge_handle(*voh_it);
            auto p2 = mesh_.point(mesh_.from_vertex_handle(heh_pre));
            p1 -= p0;
            p2 -= p0;

            auto p1_norm = p1.norm();
            auto p2_norm = p2.norm();
            auto denorm = p1_norm*p2_norm;
            if(denorm > std::numeric_limits<double>::min()) {
                auto cos = std::min(1.0, std::max(-1.0, (p1|p2)/denorm));

                auto angle = std::acos(cos);

                auto cross = p1%p2;
                auto cross_norm = cross.norm();
                if(cross_norm > std::numeric_limits<double>::min()) {
                    cross  *= angle/cross_norm;
                    nn += cross;
                }
            }
        }
        if(nn.norm() > std::numeric_limits<double>::min())
            nn.normalize();
        mesh_.property(vertex_normal_, vh) = nn;
    }
    // ------------- IMPLEMENT HERE ---------
    update_normals();
}
// ========================================================================
// EXERCISE 2.1
// ========================================================================
void Curvature::calc_uniform_laplacian() {
    min_curvature_ = DBL_MAX;
    max_curvature_ = -DBL_MAX;
    // ------------- IMPLEMENT HERE ---------
    // For each non-boundary vertex, compute uniform Laplacian operator vector
    // and store the vector length in the vertex property of the
    // mesh called vertex_curvature_.
    // Store min and max values of vertex_curvature_ in min_curvature_ and max_curvature_.
    Point laplace(0., 0., 0.);
    for (auto vh: mesh_.vertices()) {
        if (!mesh_.is_boundary(vh)) {
            laplace = Point(0.0);
            double n = 0;
            for(auto vv_it = mesh_.vv_iter(vh); vv_it.is_valid(); ++vv_it) {
                laplace += (mesh_.point(*vv_it) - mesh_.point(vh));
                n++;
            }

            laplace /= n;

            mesh_.property(vertex_curvature_, vh) = laplace.norm();
            if(mesh_.property(vertex_curvature_, vh) < min_curvature_)
                min_curvature_ = mesh_.property(vertex_curvature_, vh);
            if(mesh_.property(vertex_curvature_, vh) > max_curvature_)
                max_curvature_ = mesh_.property(vertex_curvature_, vh);
        }
    }
    // ------------- IMPLEMENT HERE ---------

    std::cout<<"Min Uniform Laplace value is: " << min_curvature_ << std::endl;
    std::cout<<"Max Uniform Laplace value is: " << max_curvature_ << std::endl;
}
// ========================================================================
// EXERCISE 2.2
// ========================================================================
void Curvature::calc_mean_curvature() {
    min_curvature_ = DBL_MAX;
    max_curvature_ = -DBL_MAX;

    calc_weights();
    // ------------- IMPLEMENT HERE ---------
    // For all non-boundary vertices, approximate the mean curvature using
    // the length of the Laplace-Beltrami approximation.
    // Save your approximation in vertex_curvature_ vertex property of the mesh.
    // Use the weights from calc_weights(): edge_weight_ and vertex_weight_
    Point laplace(0., 0., 0.);
    for (auto vh: mesh_.vertices()) {
        if (!mesh_.is_boundary(vh)) {
            laplace = Point(0.0);
            for(auto vih_it = mesh_.vih_iter(vh); vih_it.is_valid(); ++vih_it) {
                auto eh = mesh_.edge_handle(*vih_it);
                laplace += mesh_.property(edge_weight_, eh) * (mesh_.point(mesh_.from_vertex_handle(*vih_it))
                            - mesh_.point(vh));
            }

            laplace *= mesh_.property(vertex_weight_, vh);

            mesh_.property(vertex_curvature_, vh) = laplace.norm();
            if(mesh_.property(vertex_curvature_, vh) < min_curvature_)
                min_curvature_ = mesh_.property(vertex_curvature_, vh);
            if(mesh_.property(vertex_curvature_, vh) > max_curvature_)
                max_curvature_ = mesh_.property(vertex_curvature_, vh);
        }
    }
    // ------------- IMPLEMENT HERE ---------

    std::cout<<"Min Laplace-Beltrami value is: " << min_curvature_ << std::endl;
    std::cout<<"Max Laplace-Beltrami value is: " << max_curvature_ << std::endl;
}
// ========================================================================
// EXERCISE 2.3
// ========================================================================
void Curvature::calc_gauss_curvature() {
    min_curvature_ = DBL_MAX;
    max_curvature_ = -DBL_MAX;

    calc_weights();
    // ------------- IMPLEMENT HERE ---------
    // For each non-boundary vertex, approximate Gaussian curvature,
    // and store it in the vertex property v_gauss_curvature.
    // Hint: When calculating angles out of dot products make sure the value
    // you pass to the acos function is between -1.0 and 1.0.
    // Use the vertex_weight_ property for the area weight.
    double angles, cos;
    Point d0, d1;
    for (auto vh: mesh_.vertices()) {
        if (!mesh_.is_boundary(vh)) {
            angles = 0.;
            for(auto vih_it = mesh_.vih_iter(vh); vih_it.is_valid(); ++vih_it) {
                auto heh_next = mesh_.next_halfedge_handle(*vih_it);
                d0 = mesh_.point(mesh_.from_vertex_handle(*vih_it)) - mesh_.point(vh);
                d1 = mesh_.point(mesh_.to_vertex_handle(heh_next)) - mesh_.point(vh);

                auto d0_norm = d0.norm();
                auto d1_norm = d1.norm();
                auto denorm = d0_norm*d1_norm;
                if(denorm > std::numeric_limits<double>::min()) {
                    cos = std::min(1.0, std::max(-1.0, (d0 | d1) / denorm));
                    angles += std::acos(cos);
                }
            }

            mesh_.property(vertex_curvature_, vh) = (2 * M_PI - angles) * 2.0f * mesh_.property(vertex_weight_, vh);
            if(mesh_.property(vertex_curvature_, vh) < min_curvature_)
                min_curvature_ = mesh_.property(vertex_curvature_, vh);
            if(mesh_.property(vertex_curvature_, vh) > max_curvature_)
                max_curvature_ = mesh_.property(vertex_curvature_, vh);
        }
    }
    // ------------- IMPLEMENT HERE ---------

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
    Point p0(0.), p1(0.), p2(0.), d0(0.), d1(0.);
    double cross;
    double w;
    for (auto eh: mesh_.edges()) {
        w = 0.0;

        heh0 = mesh_.halfedge_handle(eh, 0);
        p0 = mesh_.point(mesh_.to_vertex_handle(heh0));

        heh1 = mesh_.halfedge_handle(eh, 1);
        p1 = mesh_.point(mesh_.to_vertex_handle(heh1));

        if (!mesh_.is_boundary(heh0))
        {
            heh2 = mesh_.next_halfedge_handle(heh0);
            p2 = mesh_.point(mesh_.to_vertex_handle(heh2));
            d0 = p0 - p2;
            d1 = p1 - p2;
            cross = std::max(1e-16, (d0 % d1).norm());
            w += (d0|d1) / cross;
        }

        if (!mesh_.is_boundary(heh1))
        {
            heh2 = mesh_.next_halfedge_handle(heh1);
            p2 = mesh_.point(mesh_.to_vertex_handle(heh2));
            d0 = p0 - p2;
            d1 = p1 - p2;
            cross = std::max(1e-16, (d0 % d1).norm());
            w += (d0|d1) / cross;
        }

        w = std::max(0., w);
        mesh_.property(edge_weight_, eh) = w;
    }
}

void Curvature::update_normals() {
    for(auto vh : mesh_.vertices())
        mesh_.set_normal(vh, 0.5*avr_e_length_*mesh_.property(vertex_normal_, vh));
}

template <typename T>
void Curvature::color_coding(const OpenMesh::VPropHandleT<T>& _vprop, const double _min_value, const double _max_value, const int _bound) {
    auto values = mesh_.property(_vprop).data_vector();
    auto min_value = _min_value;
    auto max_value = _max_value;

    if(min_value == 0 && max_value == 0) {
        // discard upper and lower bound
        auto n = values.size()-1;
        auto i = n / _bound;

        std::sort(values.begin(), values.end());
        min_value = values[i];
        max_value = values[n-1-i];
    }
//    std::cerr<<"\nmax1 "<<max_value<<" "<<min_value<<" ";

    const auto range = max_value - min_value;
    color_.set_range(0, 1.0, false);

    for(auto vh : mesh_.vertices()) {
        auto t = (mesh_.property(_vprop, vh) - min_value)/range;
        mesh_.set_color(vh, color_.color_float4(t));
    }
}
