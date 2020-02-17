#include <queue>
#include "Remeshing.hh"

void Remeshing::remesh(const int _remeshing_type, const int _num_iterations, const int _target_length_ratio) {
    //precompute curvature for calculating adaptive edge length
    calc_mean_curvature();
    for(auto vh : mesh_.vertices())
        mesh_.property(mean_curvature_, vh) = mesh_.property(vertex_curvature_, vh);

    calc_uniform_laplacian();
    for(auto vh : mesh_.vertices())
        mesh_.property(uni_curvature_, vh) = mesh_.property(vertex_curvature_, vh);

    calc_gauss_curvature();
    for(auto vh : mesh_.vertices())
        mesh_.property(gauss_curvature_, vh) = mesh_.property(vertex_curvature_, vh);

    calc_target_length(_remeshing_type, _target_length_ratio);

    //main remeshing loop
    for(int i = 0; i < _num_iterations; ++i)
    {
        split_long_edges();
        collapse_short_edges();
        equalize_valences();
        tangential_relaxation();
    }

    mesh_.update_normals();
}

void Remeshing::tangential_relaxation() {
    int valence(0);
    Point u, n;
    Point laplace;

    OpenMesh::VPropHandleT<Point> update;
    mesh_.add_property(update);
    for (auto vh : mesh_.vertices())
        mesh_.property(update, vh) = Point(0.);

    // smooth
    for (int iters = 0; iters < 10; ++iters) {
        for (auto vh : mesh_.vertices()) {
            if (!mesh_.is_boundary(vh)) {
                // ------------- IMPLEMENT HERE ---------
                //  Compute uniform laplacian curvature approximation vector
                //  Compute the tangential component of the laplacian vector and move the vertex
                //  Store smoothed vertex location in the update vertex property.

                // ------------- IMPLEMENT HERE ---------
            }
        }

        for (auto vh : mesh_.vertices()) {
            if (!mesh_.is_boundary(vh))
                mesh_.point(vh) += mesh_.property(update, vh);
        }
    }

    for(auto eh : mesh_.edges())
        mesh_.property(edge_length_, eh) = mesh_.calc_edge_length(eh);
}

void Remeshing::calc_target_length(const int _remeshing_type, const int _target_length_ratio) {
    double length, mean_length, H, K;

    std::vector<double> target_new_length(mesh_.n_vertices(), 0.);

    const float TARGET_LENGTH = _target_length_ratio * average_edge_length();
//    const float TARGET_LENGTH = 2.;

    if(_remeshing_type == 0) {
        for(auto vh : mesh_.vertices())
            mesh_.property(target_length_, vh) = TARGET_LENGTH;
    }
    else if (_remeshing_type == 1) {
        // ------------- IMPLEMENT HERE ---------
        // Get the maximal curvature at each vertex (use the precomputed mean and gaussian curvature)
        // Calculate the desired edge length as the TARGET_LENGTH divided by the maximal curvature at each vertex,
        // and assign it to the property target_length
        // Smooth the maximal curvature uniformly (5 iterations should be enough),
        // use the target_new_length to store the smoothed values intermediately
        // Rescale the property target_new_length such that it's mean equals the user specified TARGET_LENGTH
        // ------------- IMPLEMENT HERE ---------

        // ------------- IMPLEMENT HERE ---------
    }
}

void Remeshing::split_long_edges() {
    // ------------- IMPLEMENT HERE ---------
    // INSERT CODE:
    //  Compute the desired length as the mean between the property target_length of two vertices of the edge
    //  If the edge is longer than 4/3 * desired length
    //		add the midpoint to the mesh
    //		set the interpolated normal and interpolated target length property to the vertex
    //		split the edge with this vertex (use openMesh function split)
    // Use a queue(or priority queue) to process edges

    // ------------- IMPLEMENT HERE ---------
}

void Remeshing::collapse_short_edges() {
    // ------------- IMPLEMENT HERE ---------
    // INSERT CODE:
    // Compute the desired length as the mean between the property target_length of two vertices of the edge
    // If the edge is shorter than 4/5 of the desired length
    //		Check if halfedge connects a boundary vertex with a non-boundary vertex. If so, don't collapse.
    //		Check if halfedges collapsible
    //		Select the halfedge to be collapsed if at least one halfedge can be collapsed
    //		Collapse the halfedge
    // Use a queue(or priority queue) to process edges

    // ------------- IMPLEMENT HERE ---------

    mesh_.garbage_collection();
}

void Remeshing::equalize_valences() {
    // ------------- IMPLEMENT HERE ---------
    //  Extract valences of the four vertices involved to an eventual flip.
    //  Compute the sum of the squared valence deviances before flip
    //  Compute the sum of the squared valence deviances after an eventual flip
    //  If valence deviance is decreased and flip is possible, flip the vertex
    // Use a queue to process edges

    // ------------- IMPLEMENT HERE ---------

    // ------------- IMPLEMENT HERE ---------
}
