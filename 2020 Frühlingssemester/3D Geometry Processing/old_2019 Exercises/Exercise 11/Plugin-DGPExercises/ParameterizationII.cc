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

    //Start from a boundary vertex. Traverse all the boundary edges
    //and map the boundary vertices onto a circle with radius _radius,
    //the interior vertices to the circle center (0,0) in XY plane.
    // ------------- IMPLEMENT HERE ---------


    // ------------- IMPLEMENT HERE ---------

    //Update the texture mesh
    for (auto vh : mesh_.vertices()) {
        auto tex_coord = mesh_.texcoord2D(vh);
        tex_mesh_.set_point(vh, Point(tex_coord[0], tex_coord[1], 0) + _origin);
    }
}


// ======================================================================
// EXERCISE 1.2 Interactively smoothing the texture
// ========================================================================
void ParameterizationII::explicitly_smooth_texture(const Point& _origin, const int _num_iters)
{
    //See the handout
    // ------------- IMPLEMENT HERE ---------


    // ------------- IMPLEMENT HERE ---------

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
    //See the handout
    // ------------- IMPLEMENT HERE ---------


    // ------------- IMPLEMENT HERE ---------

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
    //See the handout
    // ------------- IMPLEMENT HERE ---------


    // ------------- IMPLEMENT HERE ---------

    mesh_.update_normals();
}