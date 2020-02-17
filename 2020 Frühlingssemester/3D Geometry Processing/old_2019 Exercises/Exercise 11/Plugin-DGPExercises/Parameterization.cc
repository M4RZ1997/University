#include "Parameterization.hh"

void Parameterization::solve_harmonic_function(const std::vector<int>& _constraint_indices,
        const std::string& _property_name) {

    const int n = mesh_.n_vertices();

    calc_edges_weights();

    Eigen::SparseMatrix<double> L(n, n);
    Eigen::MatrixXd rhs(Eigen::MatrixXd::Zero(n, 1));
    std::vector< Eigen::Triplet<double> > triplets_L;

    for (auto vh : mesh_.vertices()) {

        // ------------- IMPLEMENT HERE ---------
        // Set up Laplace-Beltrami matrix of the mesh
        // For the vertices for which the constraints are added, replace the corresponding row of the system with the constraint
        // ------------- IMPLEMENT HERE ---------

        if (vh.idx() == _constraint_indices[0] || vh.idx() == _constraint_indices[1]) {
            triplets_L.push_back(Eigen::Triplet<double>(vh.idx(), vh.idx(), 1));
            if (vh.idx() == _constraint_indices[0])
                rhs(vh.idx(), 0) = 1;
            if (vh.idx() == _constraint_indices[1])
                rhs(vh.idx(), 0) = 0;
            continue;
        }

        rhs(vh.idx(), 0) = 0;
        double cotan_sum = 0.0;
        for(auto voh_it = mesh_.voh_iter(vh); voh_it.is_valid(); ++voh_it) {
            auto vh_t = mesh_.to_vertex_handle(*voh_it);
            auto eh = mesh_.edge_handle(*voh_it);
            cotan_sum += mesh_.property(edge_weight_, eh);
            triplets_L.push_back(Eigen::Triplet<double>(vh.idx(), vh_t.idx(), mesh_.property(edge_weight_, eh)));
        }
        triplets_L.push_back(Eigen::Triplet<double>(vh.idx(), vh.idx(), -cotan_sum));

    }

    L.setFromTriplets(triplets_L.begin(), triplets_L.end());
    Eigen::SparseLU< Eigen::SparseMatrix<double> > solver(L);
    if (solver.info() != Eigen::Success) {
        printf("linear solver init failed.\n");
        return;
    }
    Eigen::MatrixXd X = solver.solve(rhs);
    if (solver.info() != Eigen::Success) {
        printf("linear solver failed.\n");
        return;
    }
    std::cout << "X[0] = " << X(_constraint_indices[0], 0) << std::endl;
    std::cout << "X[1] = " << X(_constraint_indices[1], 0) << std::endl;

    OpenMesh::VPropHandleT<double> v_harmonic_function;
    if(!mesh_.get_property_handle(v_harmonic_function, _property_name)) {
        mesh_.add_property(v_harmonic_function, _property_name);
        mesh_.property(v_harmonic_function).set_persistent(true);
    }
    for (auto vh : mesh_.vertices())
        mesh_.property(v_harmonic_function, vh) = X(vh.idx(), 0);

//    auto values = mesh_.property(v_harmonic_function).data_vector();
    color_coding(v_harmonic_function);
}

std::pair<int, int> Parameterization::get_intervals_borders(const double _iso0, const double _iso1,
        double _lower_bound, double _interval_size) {

    std::pair<int, int> intervals_borders;

    // ------------- IMPLEMENT HERE ---------
    // Given the values of the harmonic function that correspond to the two vertices in a triangle,
    // find the first and the last interval border that fall between the isovalues at the two vertices
    // Use std::pair to return the indices of the first and the last interval border.
    // ------------- IMPLEMENT HERE ---------
    auto min_value = std::min(_iso0, _iso1);
    auto max_value = std::max(_iso0, _iso1);
    intervals_borders .first = ceil((min_value - _lower_bound) / _interval_size);
    intervals_borders.second = floor((max_value - _lower_bound) / _interval_size);

    return intervals_borders;
}

void Parameterization::add_isoline_segment(const std::pair<int, int>& _borders01, const std::pair<int, int>& _borders02,
        const double _iso0, const double _iso1, const double _iso2, const Point& _p0, const Point& _p1, const Point& _p2,
        double _lower_bound, double _interval_size) {

    // ------------- IMPLEMENT HERE ---------
    // For each two edges of a triangle check if they are intersected by the same isoline.
    // If this is the case, compute the intersections using linear interpolation of the isovalues.
    // Add an isoline segment when the isoline indices for the two edges coincide
    // (isolines_points_.push_back(p0); isolines_points_.push_back(p1);)
    // ------------- IMPLEMENT HERE ---------

    int interval_index_min = std::max(_borders01.first, _borders02.first);
    int interval_index_max = std::min(_borders01.second, _borders02.second);

    for (int interval_index = interval_index_min; interval_index <= interval_index_max; interval_index++) {
        float interval_boarder = _lower_bound + interval_index * _interval_size;

        float shift0 = std::abs(interval_boarder - _iso0) / (std::abs(_iso0 - _iso1));
        Point p0 = _p0 + shift0 * (_p1 - _p0);
        segment_points_.push_back(p0);

        float shift1 = std::abs(interval_boarder - _iso0) / (std::abs(_iso0 - _iso2));
        Point p1 = _p0 + shift1 * (_p2 - _p0);
        segment_points_.push_back(p1);
    }
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

//    if(vhs.size() % 2 != 0)
//        std::cout<<"\nError: one segment has only one point!";

    for(size_t i=0; i<vhs.size()/2; ++i)
        _line_mesh.add_edge(vhs[2*i], vhs[2*i + 1]);
}