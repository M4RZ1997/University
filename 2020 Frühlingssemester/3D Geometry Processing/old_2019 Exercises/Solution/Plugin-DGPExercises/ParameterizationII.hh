#ifndef OPENFLIPPER_PARAMETERIZATIONII_HH
#define OPENFLIPPER_PARAMETERIZATIONII_HH

#include <unordered_map>
#include "./eigen3/Eigen/Sparse"
#include "Curvature.hh"

class ParameterizationII : public Curvature {
public:
    using Vec2d = TriMesh::TexCoord2D;

public:
    ParameterizationII(TriMesh& _mesh, TriMesh& _tex_mesh) : Curvature(_mesh),
            mesh_(_mesh), tex_mesh_(_tex_mesh) {
        //get edge_weight_ property handle
        mesh_.get_property_handle(edge_weight_, "edge weights");
    }

    ~ParameterizationII(){}

public:
    void map_suface_boundary_to_circle(const Point& _origin, const double _radius = 10.);

    void explicitly_smooth_texture(const Point& _origin, const int _num_iters = 10);

    void implicitly_smooth_texture(const Point& _origin);

    void minimal_surface();

private:
    OpenMesh::HalfedgeHandle previous_boundary_halfedge_handle(OpenMesh::HalfedgeHandle _heh) const;

private:
    TriMesh& mesh_;
    TriMesh& tex_mesh_;
    OpenMesh::EPropHandleT<double> edge_weight_;
};

#endif //OPENFLIPPER_PARAMETERIZATIONII_HH
