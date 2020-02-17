#include "ParameterizationII.hh"
#include <ACG/Utils/StopWatch.hh>

void ParameterizationII::map_suface_boundary_to_circle(const Point& _origin, const double _radius) {
    mesh_.request_vertex_texcoords2D();
    for(auto vh : mesh_.vertices())
        mesh_.set_texcoord2D(vh, Vec2d(0., 0.));

    //copy mesh_ to tex_mesh_
    for(auto vh : mesh_.vertices())
        tex_mesh_.add_vertex(mesh_.point(vh));
    for(auto fh : mesh_.faces()) {
        std::vector<OpenMesh::VertexHandle> fh_vhs;
        for(auto fv_it = mesh_.fv_iter(fh); fv_it.is_valid(); ++fv_it)
            fh_vhs.push_back(*fv_it);

        tex_mesh_.add_face(fh_vhs);
    }

    //Finding the boundary
    OpenMesh::VertexHandle vh_start;
    for(auto vh : mesh_.vertices()) {
        if(mesh_.is_boundary(vh)) {
            //start from a manifold vertex and valence larger than 2
            if(mesh_.is_manifold(vh) && mesh_.valence(vh) > 2){
                vh_start = vh;
                break;
            }
        }
    }
    if(!vh_start.is_valid()) {
        std::cerr<<"\nProbably the input mesh has no boundary!";
        return;
    }

    double sum_rads = 0.;
    std::unordered_map<int, bool> visited;

    //get the start halfedge
    OpenMesh::HalfedgeHandle h_it;
    for(auto vih_it = mesh_.vih_iter(vh_start); vih_it.is_valid(); ++vih_it)
        if(mesh_.is_boundary(*vih_it))
            h_it = *vih_it;

    auto v_it = vh_start;
    //ccw
    do {
        v_it = mesh_.from_vertex_handle(h_it);
        if(visited[v_it.idx()])
            break;

        visited[v_it.idx()] = true;
        sum_rads += mesh_.calc_edge_length(h_it);
        mesh_.set_texcoord2D(v_it, Vec2d(sum_rads, sum_rads));

        h_it = previous_boundary_halfedge_handle(h_it);
    } while (h_it.is_valid());

    //map the visited boundary vertices onto a circle
    for(auto vh : mesh_.vertices()) {
        if(mesh_.is_boundary(vh) && visited[vh.idx()]){
            mesh_.set_texcoord2D(vh, Vec2d(std::cos(mesh_.texcoord2D(vh)[0]/sum_rads * 2 * M_PI)*_radius,
                    std::sin(mesh_.texcoord2D(vh)[1]/sum_rads * 2 * M_PI)*_radius));
        }
    }

    //Update the texture mesh
    for (auto vh : mesh_.vertices()) {
        auto tex_coord = mesh_.texcoord2D(vh);
        tex_mesh_.set_point(vh, Point(tex_coord[0], tex_coord[1], 0) + _origin);
    }
}

OpenMesh::HalfedgeHandle ParameterizationII::previous_boundary_halfedge_handle(OpenMesh::HalfedgeHandle _heh) const {
    if(!mesh_.is_boundary(_heh)) {
        std::cerr<<"\nError: Input halfedge is not boundary!";
        return OpenMesh::HalfedgeHandle(-1);
    }
    auto vh = mesh_.from_vertex_handle(_heh);
    auto h_it = _heh;
    do {
        h_it = mesh_.opposite_halfedge_handle(h_it);
        h_it = mesh_.next_halfedge_handle(h_it);
    } while (!mesh_.is_boundary(mesh_.opposite_halfedge_handle(h_it)));

    return mesh_.opposite_halfedge_handle(h_it);
}

// ======================================================================
// EXERCISE 1.2 Interactively smoothing the texture
// ========================================================================
void ParameterizationII::explicitly_smooth_texture(const Point& _origin, const int _num_iters)
{
    int n_vertices = mesh_.n_vertices();

    calc_edges_weights();

    std::vector<Vec2d> v_texture_prev;
    v_texture_prev.reserve(n_vertices);
    for(int i=0; i<_num_iters; ++i) {
        v_texture_prev.clear();
        //Homework stopping from here
        for(auto vh : mesh_.vertices()){
            double  u = mesh_.texcoord2D(vh)[0];
            double  v = mesh_.texcoord2D(vh)[1];
            v_texture_prev.push_back(Vec2d(u, v));
        }

        for(auto vh : mesh_.vertices()){
            Vec2d tex_coord(0, 0);
            if(!mesh_.is_boundary(vh))
            {
                double cotan_sum = 0.0;
                for (auto voh_it = mesh_.voh_iter(vh); voh_it.is_valid(); ++voh_it) {
                    auto vh_t = mesh_.to_vertex_handle(*voh_it);
                    auto eh = mesh_.edge_handle(*voh_it);
                    cotan_sum += mesh_.property(edge_weight_, eh);
                    tex_coord +=  v_texture_prev[vh_t.idx()] * mesh_.property(edge_weight_, eh);
                }
                mesh_.set_texcoord2D(vh, tex_coord/cotan_sum);
            }
        }
    }

    //Update the texture mesh
    for (auto vh : mesh_.vertices()) {
        auto tex_coord = mesh_.texcoord2D(vh);
            tex_mesh_.set_point(vh, Point(tex_coord[0], tex_coord[1], 0) + _origin);
    }
}


// ======================================================================
// EXERCISE 1.3 Implicitly smoothing the texture
// ========================================================================
void ParameterizationII::implicitly_smooth_texture(const Point& _origin)
{
    calc_edges_weights();

    std::vector< Eigen::Triplet<double> > triplets_L;
    int j = 0;
    std::vector<int> new_index;
    for(auto vh : mesh_.vertices()) {
        if(!mesh_.is_boundary(vh))
            new_index.push_back(j++);
        else
            new_index.push_back(-1);
    }

    //set up matrix and rhs
    Eigen::MatrixXd rhs(Eigen::MatrixXd::Zero(j, 2));

    int non_boundary_num = j;
    for (auto vh : mesh_.vertices()) {
        if(new_index[vh.idx()] == -1)
            continue;

        double cotan_sum = 0.0;
        for (auto voh_it = mesh_.voh_iter(vh); voh_it.is_valid(); ++voh_it) {
            auto vh_t = mesh_.to_vertex_handle(*voh_it);
            auto eh = mesh_.edge_handle(*voh_it);
            cotan_sum += mesh_.property(edge_weight_, eh);
            //rhs
            if(new_index[vh_t.idx()] == -1) {
                rhs(new_index[vh.idx()], 0) -= mesh_.property(edge_weight_, eh) * mesh_.texcoord2D(vh_t)[0];
                rhs(new_index[vh.idx()], 1) -= mesh_.property(edge_weight_, eh) * mesh_.texcoord2D(vh_t)[1];
            }
            //matrix
            if(new_index[vh_t.idx()] != -1)
                triplets_L.push_back(Eigen::Triplet<double>(new_index[vh.idx()], new_index[vh_t.idx()],
                        mesh_.property(edge_weight_, eh)));
        }
        //matrix
        triplets_L.push_back(Eigen::Triplet<double>(new_index[vh.idx()], new_index[vh.idx()], -cotan_sum));
    }

    Eigen::SparseMatrix<double> L(non_boundary_num, non_boundary_num);
    L.setFromTriplets(triplets_L.begin(), triplets_L.end());

    //solve
    Eigen::SimplicialLDLT< Eigen::SparseMatrix<double>> solver(L);
    if (solver.info() != Eigen::Success)
    {
        printf("linear solver init failed.\n");
    }


    Eigen::MatrixXd X = solver.solve(rhs);
    std::cout << (L * X - rhs).norm() << std::endl;
    if (solver.info() != Eigen::Success) {
        printf("linear solver failed.\n");
    }

    //store result
    for(auto vh : mesh_.vertices())
        if(!mesh_.is_boundary(vh))
            mesh_.set_texcoord2D(vh, Vec2d(X(new_index[vh.idx()], 0), X(new_index[vh.idx()], 1)));

    //Update the texture mesh
    for (auto vh : mesh_.vertices()) {
        auto tex_coord = mesh_.texcoord2D(vh);
        tex_mesh_.set_point(vh, Point(tex_coord[0], tex_coord[1], 0) + _origin);
    }
}


// ======================================================================
// EXERCISE 2 Minimal Surfaces
// ======================================================================
void ParameterizationII::minimal_surface() {
    calc_edges_weights();

    // A*X = B
    Eigen::SparseMatrix<double> L (mesh_.n_vertices(), mesh_.n_vertices());
    Eigen::MatrixXd rhs (Eigen::MatrixXd::Zero (mesh_.n_vertices(), 3));

    // nonzero elements of A as triplets: (row, column, value)
    std::vector<Eigen::Triplet<double> > triplets_L;

    // setup matrix A and rhs B
    for (auto vh : mesh_.vertices()) {
        if (mesh_.is_boundary(vh) ) {
            triplets_L.push_back(Eigen::Triplet<double> (vh.idx(), vh.idx(), 1.));

            // rhs row -- all equal to original
            for (int dim = 0; dim < 3; ++dim) {
                rhs(vh.idx(), dim) = mesh_.point(vh)[dim];
            }
        } else {
            // rhs row -- all equal to zero
            for (int dim = 0; dim < 3; ++dim) {
                rhs(vh.idx(), dim) = 0;
            }

            // lhs row
            double cotan_sum = 0.0;
            for (auto voh_it = mesh_.voh_iter(vh); voh_it.is_valid(); ++voh_it) {
                auto vh_t = mesh_.to_vertex_handle(*voh_it);
                auto eh = mesh_.edge_handle(*voh_it);
                cotan_sum += mesh_.property(edge_weight_, eh);

                triplets_L.push_back(Eigen::Triplet<double>(vh.idx(), vh_t.idx(), mesh_.property(edge_weight_, eh)));
            }
            triplets_L.push_back(Eigen::Triplet<double>(vh.idx(), vh.idx(), -cotan_sum));
        }
    }

    L.setFromTriplets (triplets_L.begin (), triplets_L.end ());

    // solve A*X = B
    Eigen::SparseLU< Eigen::SparseMatrix<double> > solver(L);
    if (solver.info () != Eigen::Success) {
        printf("linear solver init failed.\n");
    }

    Eigen::MatrixXd X = solver.solve(rhs);
    if (solver.info () != Eigen::Success) {
        printf("linear solver failed.\n");
    }

    //store result
    for(auto vh : mesh_.vertices())
        if(!mesh_.is_boundary(vh))
            mesh_.point(vh) = Point(X(vh.idx(), 0), X(vh.idx(), 1), X(vh.idx(), 2));

    mesh_.update_normals();
}