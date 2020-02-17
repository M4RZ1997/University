#include "Parameterization.hh"

void Parameterization::solve_harmonic_function(const std::vector<int>& _constraint_indices,
        const std::string& _property_name) {
    // ------------- IMPLEMENT HERE ---------
    // Set up Laplace-Beltrami matrix of the mesh
    // For the vertices for which the constraints are added,
    // replace the corresponding row of the system with the constraint
    // Store the result in vertex property v_harmonic_function.
    // ------------- IMPLEMENT HERE ---------

    // --------------------------------------
    OpenMesh::VPropHandleT<double> v_harmonic_function;
    if(!mesh_.get_property_handle(v_harmonic_function, _property_name)) {
        mesh_.add_property(v_harmonic_function, _property_name);
        mesh_.property(v_harmonic_function).set_persistent(true);
    }

    // ------------- STORE RESULT HERE ---------

    // --------------------------------------

    color_coding(v_harmonic_function);
}

std::pair<int, int> Parameterization::get_intervals_borders(const double _iso0, const double _iso1,
        double _lower_bound, double _interval_size) {

    std::pair<int, int> intervals_borders;

    // ------------- IMPLEMENT HERE ---------
    // Given the values of the harmonic function that correspond to the two vertices in a triangle,
    // find the first and the last interval border that fall between the isovalues at the two vertices
    // Use std::pair to return the indices of the first and the last interval border.


    // --------------------------------------

    return intervals_borders;
}

void Parameterization::add_isoline_segment(const std::pair<int, int>& _borders01, const std::pair<int, int>& _borders02,
        const double _iso0, const double _iso1, const double _iso2, const Point& _p0, const Point& _p1, const Point& _p2,
        double _lower_bound, double _interval_size) {

    // ------------- IMPLEMENT HERE ---------
    // For each two edges of a triangle check if they are intersected by the same isoline.
    // If this is the case, compute the intersections using linear interpolation of the isovalues.
    // Add an isoline segment when the isoline indices for the two edges coincide
    // (segment_points_.push_back(p0); segment_points_.push_back(p1);)


    // --------------------------------------
}

void Parameterization::compute_isolines(TetrahedralMesh& _line_mesh, const std::vector<int>& _constraint_indices,
        const std::string& _property_name, int _num_intervals) {
    segment_points_.clear();

    OpenMesh::VPropHandleT<double> v_harmonic_function;
    if(mesh_.get_property_handle(v_harmonic_function, _property_name)) {
        auto lower_bound = mesh_.property(v_harmonic_function, OpenMesh::VertexHandle(_constraint_indices[1]));
        auto upper_bound = mesh_.property(v_harmonic_function, OpenMesh::VertexHandle(_constraint_indices[0]));
        auto interval_size = (upper_bound - lower_bound) / (double)_num_intervals;

        for (auto fh : mesh_.faces()) {
            std::vector<Point> pts;
            std::vector<double> iso_vals;
            for (auto fv_it = mesh_.fv_iter(fh); fv_it.is_valid(); ++fv_it) {
                iso_vals.push_back(mesh_.property(v_harmonic_function, *fv_it));
                pts.push_back(mesh_.point(*fv_it));
            }

            std::pair<int, int> borders01 = get_intervals_borders(iso_vals[0], iso_vals[1], lower_bound, interval_size);
            std::pair<int, int> borders12 = get_intervals_borders(iso_vals[1], iso_vals[2], lower_bound, interval_size);
            std::pair<int, int> borders02 = get_intervals_borders(iso_vals[0], iso_vals[2], lower_bound, interval_size);

            add_isoline_segment(borders01, borders02, iso_vals[0], iso_vals[1], iso_vals[2], pts[0], pts[1], pts[2], lower_bound, interval_size);
            add_isoline_segment(borders01, borders12, iso_vals[1], iso_vals[0], iso_vals[2], pts[1], pts[0], pts[2], lower_bound, interval_size);
            add_isoline_segment(borders02, borders12, iso_vals[2], iso_vals[0], iso_vals[1], pts[2], pts[0], pts[1], lower_bound, interval_size);
        }
    }

    //create mesh
    std::vector<OpenVolumeMesh::VertexHandle> vhs;
    for(auto pt : segment_points_)
        vhs.push_back(_line_mesh.add_vertex(pt));

    for(size_t i=0; i<vhs.size()/2; ++i)
        _line_mesh.add_edge(vhs[2*i], vhs[2*i + 1]);
}