#ifndef OPENFLIPPER_DEFORMATION_HH
#define OPENFLIPPER_DEFORMATION_HH

#include "./eigen3/Eigen/Sparse"
#include "Curvature.hh"

class Deformation : public Curvature {
public:
    Deformation(TriMesh& _mesh) :
            Curvature(_mesh), mesh_(_mesh) {
        //get vertex_weight_ property handle
        mesh_.get_property_handle(edge_weight_, "edge weights");
    }

    ~Deformation(){}

public:
    void deform(const std::vector<int>& _fixed_vts, const std::vector<int>& _displaced_vts,
            const Point& _displacement);


private:
    TriMesh& mesh_;
    OpenMesh::EPropHandleT<double> edge_weight_;
};

#endif //OPENFLIPPER_DEFORMATION_HH
