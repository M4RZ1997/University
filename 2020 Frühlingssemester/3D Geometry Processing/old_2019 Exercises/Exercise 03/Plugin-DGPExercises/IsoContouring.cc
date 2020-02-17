#include "IsoContouring.hh"
#include <math.h>

void
IsoContouring::show_isovalue_and_level_set() {
    show_isovalue();
    create_level_set0_segments();
}

//====================================================================================================================//

void
IsoContouring::show_isovalue() {
    std::vector<double> iso_values;
    for(auto vh : trimesh_.vertices()) {
        auto point = trimesh_.point(vh);
        auto iv = iso_value(point);

        iso_values.push_back(iv);
    }

    auto max_iv = *std::max_element(iso_values.begin(), iso_values.end());
    auto min_iv = *std::min_element(iso_values.begin(), iso_values.end());

    const double range = max_iv - min_iv;
    color_.set_range(0, 1.0, false);

    for(auto vh : trimesh_.vertices()) {
        auto t = (iso_values[vh.idx()] - min_iv)/range;
        trimesh_.set_color(vh, color_.color_float4(t));
    }
}


double
IsoContouring::iso_value(const Point& _pt) const {
    auto x = _pt[0];
    auto y = _pt[1];
    double iso = 0.;

    //Four functions are given in the handout
    //----------Add your code here----------
    switch(function_id_) {
        case 0: {
            //Function 1
            break;
        }
        case 1: {
            //Function 2
            break;
        }
        case 2: {
            //Function 3
            break;
        }
        default: {
            //Function 4
            break;
        }
    }
    //--------------------------------------

    return iso;
}


void
IsoContouring::create_level_set0_segments() {
    compute_segment_points();

    //create segments
    std::vector<OpenVolumeMesh::VertexHandle> vhs;
    for(auto pt : segment_points_)
        vhs.push_back(line_mesh_.add_vertex(pt));

    if(vhs.size() % 2 != 0)
        std::cout<<"\nError: one segment has only one point!";

    for(size_t i=0; i<vhs.size()/2; ++i)
        line_mesh_.add_edge(vhs[2*i], vhs[2*i + 1]);
}


void
IsoContouring::compute_segment_points() {
    segment_points_.clear();

    std::vector<Point> v_points(trimesh_.n_vertices());
    std::vector<std::vector<int> > triangle_ids;

    //store vertex position in v_points
    for(auto vh : trimesh_.vertices()) {
        Point v_pos = trimesh_.point(vh);
        v_points[vh.idx()] = v_pos;
    }

    //store vertex indices in triangle_ids
    for(auto fh : trimesh_.faces()) {
        std::vector<int> vv(3);
        int k = 0;
        for (auto fv_it = trimesh_.fv_ccwbegin(fh); fv_it != trimesh_.fv_ccwend(fh); ++fv_it) {
            vv[k] = (*fv_it).idx();
            k++;
        }
        triangle_ids.push_back(vv);
    }

    //segment_points_ is defined in IsoContouring.hh as std::vector<Point> segment_points_;
    //add points in segment_points forming an edge one after the other;
    //for example segment_points[0] and segment_points[1] are two points forming the first edge
    //and segment_points[2] and segment_points[3] are two points forming the second edge

    //----------Add your code here----------


    //--------------------------------------

}
