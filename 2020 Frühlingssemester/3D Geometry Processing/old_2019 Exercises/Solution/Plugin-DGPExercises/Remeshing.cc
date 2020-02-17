#include <queue>
#include "Remeshing.hh"

void Remeshing::remesh(const int _remeshing_type, const int _num_iterations, const int _target_length_ratio) {
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

    // smooth
    for (int iters = 0; iters < 10; ++iters) {
        for (auto vh : mesh_.vertices()) {
            if (!mesh_.is_boundary(vh)) {
                // ------------- IMPLEMENT HERE ---------
                //  Compute uniform laplacian curvature approximation vector
                //  Compute the tangential component of the laplacian vector and move the vertex
                //  Store smoothed vertex location in the update vertex property.

                u = Point(0.0);
                valence = 0;

                for (auto vv_it = mesh_.vv_iter(vh); vv_it.is_valid(); ++vv_it) {
                    u += mesh_.point(*vv_it);
                    ++valence;
                }
                if (valence) {
                    laplace = u / (double) valence - mesh_.point(vh);
                    n = mesh_.property(vertex_normal_, vh);
                    laplace -= n * (n | laplace);
                }

                mesh_.property(update, vh) = laplace;
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
        // Calculate the desired edge length as the TARGET_LENGTH divided by the maximal curvature at each vertex, and assign it to the property target_length
        // Smooth the target length uniformly, use the target_new_length to store the smoothed values intermediately
        // Rescale the property target_new_length such that it's mean equals the user specified TARGET_LENGTH
        // ------------- IMPLEMENT HERE ---------
        //calculate desired length
        for (auto vh : mesh_.vertices())
        {
            length = 1.0;
            if (!mesh_.is_boundary(vh))
            {
                H = mesh_.property(mean_curvature_, vh);
                K = mesh_.property(gauss_curvature_, vh);
                if ((H*H - K) >= 0)
                    length = TARGET_LENGTH / (H + sqrt(H*H - K));
                else
                    length = TARGET_LENGTH / H;
            }
            mesh_.property(target_length_, vh) = length;
        }

        //smooth desired length
        for (int i = 0; i < 5; i++) {
            for (auto vh : mesh_.vertices()) {
                length = 0.0;
                for(auto vv_it = mesh_.vv_iter(vh); vv_it.is_valid(); ++vv_it)
                    length += mesh_.property(target_length_, *vv_it);

                target_new_length[vh.idx()] = length / mesh_.valence(vh);
            }

            for (auto vh : mesh_.vertices())
                mesh_.property(target_length_, vh) = target_new_length[vh.idx()];
        }

        //rescale desired length:
        mean_length = 0.f;
        for (auto vh : mesh_.vertices())
            mean_length += mesh_.property(target_length_, vh);
        mean_length /= mesh_.n_vertices();

        auto scaling_factor = TARGET_LENGTH / mean_length;

        for (auto vh : mesh_.vertices())
            mesh_.property(target_length_, vh) *= scaling_factor;
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
    // Leave the loop running until the queue is empty
    std::priority_queue<PairLE, std::vector<PairLE>, std::less<PairLE>> edge_que;
    for(auto eh : mesh_.edges())
        edge_que.push(std::make_pair(mesh_.property(edge_length_, eh), eh));
//    int n=0;
    while(!edge_que.empty()) {
        auto le_cur = edge_que.top();
        edge_que.pop();

        auto vh0 = mesh_.to_vertex_handle(mesh_.halfedge_handle(le_cur.second, 0));
        auto vh1 = mesh_.to_vertex_handle(mesh_.halfedge_handle(le_cur.second, 1));

        auto length = (mesh_.property(target_length_, vh0) + mesh_.property(target_length_, vh1)) * 0.5;
        if(le_cur.first > 4.f/3.f*length) {
            auto new_vh = mesh_.add_vertex((mesh_.point(vh0) + mesh_.point(vh1)) * 0.5);
            mesh_.split_edge(le_cur.second, new_vh);
//            n++;
            mesh_.property(vertex_normal_, new_vh) = (mesh_.property(vertex_normal_, vh0)
                    + mesh_.property(vertex_normal_, vh1)).normalize();
            mesh_.property(target_length_, new_vh) = length;

            for(auto voh_it = mesh_.voh_iter(new_vh); voh_it.is_valid(); ++voh_it) {
                auto ehi = mesh_.edge_handle(*voh_it);
                //update edge length
                mesh_.property(edge_length_, ehi) = mesh_.calc_edge_length(*voh_it);
                //push the new edges into the queue
                edge_que.push(std::make_pair(mesh_.property(edge_length_, ehi), ehi));
            }
        }
    }
//    std::cerr<<"\nsplit "<<n<<"edges";
    // ------------- IMPLEMENT HERE ---------
}

void Remeshing::collapse_short_edges() {
    std::priority_queue<PairLE, std::vector<PairLE>, std::greater<PairLE>> edge_que;
    for(auto eh : mesh_.edges())
        edge_que.push(std::make_pair(mesh_.property(edge_length_, eh), eh));
    // ------------- IMPLEMENT HERE ---------
    // INSERT CODE:
    // Compute the desired length as the mean between the property vtargetlength_ of two vertices of the edge
    // If the edge is shorter than 4/5 of the desired length
    //		Check if halfedge connects a boundary vertex with a non-boundary vertex. If so, don't collapse.
    //		Check if halfedges collapsible
    //		Select the halfedge to be collapsed if at least one halfedge can be collapsed
    //		Collapse the halfedge
    // Leave the loop running until the queue is empty
//    int n=0;
    while(!edge_que.empty()) {
        auto le_cur = edge_que.top();
        edge_que.pop();

        if(le_cur.first != mesh_.property(edge_length_, le_cur.second))
            continue;

        auto heh0 = mesh_.halfedge_handle(le_cur.second, 0);
        auto heh1 = mesh_.halfedge_handle(le_cur.second, 1);
        auto vh0 = mesh_.from_vertex_handle(heh0);
        auto vh1 = mesh_.to_vertex_handle(heh0);

        auto length = (mesh_.property(target_length_, vh0) + mesh_.property(target_length_, vh1)) * 0.5;

        if(le_cur.first < 4.f/5.f*length) {
            // get boundary status
            auto colh01 = true, colh10 = true;
            auto b0 = mesh_.is_boundary(vh0);
            auto b1 = mesh_.is_boundary(vh1);

            // boundary rules (dont collapse boundary to interior)
            if(b0 && b1){
                if(!mesh_.is_boundary(le_cur.second))
                    continue;
            } else if(b0)
                colh01 = false;
            else if(b1)
                colh10 = false;

            //topological check
            if(colh01)
                colh01 = mesh_.is_collapse_ok(heh0);
            if(colh10)
                colh10 = mesh_.is_collapse_ok(heh1);

            // both collapses possible: collapse into vertex with higher valence
            if(colh01 && colh10) {
                if (mesh_.valence(vh0) < mesh_.valence(vh1))
                    colh10 = false;
                else
                    colh01 = false;
            }

            // try v0 -> v1
            if (colh01) {
                mesh_.collapse(heh0);
                //push the new edges into the queue
                for(auto voh_it = mesh_.voh_iter(vh1); voh_it.is_valid(); ++voh_it) {
                    auto ehi = mesh_.edge_handle(*voh_it);
                    //update edge length
                    mesh_.property(edge_length_, ehi) = mesh_.calc_edge_length(*voh_it);
                    //push the new edges into the queue
                    edge_que.push(std::make_pair(mesh_.property(edge_length_, ehi), ehi));
                }
//                n++;
            }
            // try v1 -> v0
            else if (colh10) {
                mesh_.collapse(heh1);
                //push the new edges into the queue
                for(auto voh_it = mesh_.voh_iter(vh0); voh_it.is_valid(); ++voh_it) {
                    auto ehi = mesh_.edge_handle(*voh_it);
                    //update edge length
                    mesh_.property(edge_length_, ehi) = mesh_.calc_edge_length(*voh_it);
                    //push the new edges into the queue
                    edge_que.push(std::make_pair(mesh_.property(edge_length_, ehi), ehi));
                }
//                n++;
            }
        }
    }
//    std::cerr<<"\ncollapse "<<n<<" edges";

    // ------------- IMPLEMENT HERE ---------

    mesh_.garbage_collection();
}

void Remeshing::equalize_valences() {
    std::queue<OpenMesh::EdgeHandle> edge_que;
    for(auto eh : mesh_.edges())
        edge_que.push(eh);
    // ------------- IMPLEMENT HERE ---------
    //  Extract valences of the four vertices involved to an eventual flip.
    //  Compute the sum of the squared valence deviances before flip
    //  Compute the sum of the squared valence deviances after an eventual flip
    //  If valence deviance is decreased and flip is possible, flip the vertex
    // Leave the loop running until the queue is empty
    // ------------- IMPLEMENT HERE ---------
    int val0 = 0, val1 = 0, val2 = 0, val3 = 0;
    std::vector<OpenMesh::VertexHandle> vhs(4);
//    int n=0;
    while(!edge_que.empty()) {
        auto eh_cur = edge_que.front();
        edge_que.pop();

        if(mesh_.is_boundary(eh_cur))
            continue;

        auto heh0 = mesh_.halfedge_handle(eh_cur, 0);
        vhs[0] = mesh_.to_vertex_handle(heh0);
        vhs[2] = mesh_.to_vertex_handle(mesh_.next_halfedge_handle(heh0));
        auto heh1 = mesh_.halfedge_handle(eh_cur, 1);
        vhs[1] = mesh_.to_vertex_handle(heh1);
        vhs[3] = mesh_.to_vertex_handle(mesh_.next_halfedge_handle(heh1));

        val0 = mesh_.valence(vhs[0]);
        val1 = mesh_.valence(vhs[1]);
        val2 = mesh_.valence(vhs[2]);
        val3 = mesh_.valence(vhs[3]);

        auto val_opt0 = (mesh_.is_boundary(vhs[0]) ? 4 : 6);
        auto val_opt1 = (mesh_.is_boundary(vhs[1]) ? 4 : 6);
        auto val_opt2 = (mesh_.is_boundary(vhs[2]) ? 4 : 6);
        auto val_opt3 = (mesh_.is_boundary(vhs[3]) ? 4 : 6);

        auto ve0 = (val0 - val_opt0);
        auto ve1 = (val1 - val_opt1);
        auto ve2 = (val2 - val_opt2);
        auto ve3 = (val3 - val_opt3);

        auto ve_before = ve0*ve0 + ve1*ve1 + ve2*ve2 + ve3*ve3;

        ve0 = (val0 - 1 - val_opt0);
        ve1 = (val1 - 1 - val_opt1);
        ve2 = (val2 + 1 - val_opt2);
        ve3 = (val3 + 1 - val_opt3);

        auto ve_after = ve0*ve0 + ve1*ve1 + ve2*ve2 + ve3*ve3;

        if (ve_before > ve_after && mesh_.is_flip_ok(eh_cur)) {
            mesh_.flip(eh_cur);
//            n++;
            //update edge length
            mesh_.property(edge_length_, eh_cur) = mesh_.calc_edge_length(eh_cur);
            //push relevant edges into the queue
            for(int i=0; i<4; ++i)
                for(auto voh_it = mesh_.voh_iter(vhs[i]); voh_it.is_valid(); ++voh_it){
                    edge_que.push(mesh_.edge_handle(*voh_it));
                    edge_que.push(mesh_.edge_handle(mesh_.next_halfedge_handle(*voh_it)));
                }
        }
    }
//    std::cerr<<"\nflip "<<n<<" edges";

    // ------------- IMPLEMENT HERE ---------
}
