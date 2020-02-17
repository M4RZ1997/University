#ifndef OPENFLIPPER_SMOOTHING_HH
#define OPENFLIPPER_SMOOTHING_HH

#include "./eigen3/Eigen/Sparse"
#include "Curvature.hh"


class Smoothing : public Curvature {
public:
    Smoothing(TriMesh& _mesh) :
    Curvature(_mesh), mesh_(_mesh) {
        //get vertex_weight_ property handle
        mesh_.get_property_handle(vertex_weight_, "vertex weights");

        //get edge_weight_ property handle
        mesh_.get_property_handle(edge_weight_, "edge weights");
    }

    ~Smoothing(){}

    //Explicit smoothing
    void uniform_smooth(const int _iterations);

    void cotan_laplacian_smooth(const int _iterations);

    //Implicit smoothing
    void implicit_smooth(const double _timestep = 1e-4);//1e-5);

    void uniform_laplacian_enhance_feature(const int _iterations, const int _coefficient);

    void cotan_laplacian_enhance_feature(const int _iterations, const int _coefficient);


private:
    TriMesh& mesh_;
    OpenMesh::VPropHandleT<double> vertex_weight_;
    OpenMesh::EPropHandleT<double> edge_weight_;
};

#endif //OPENFLIPPER_SMOOTHING_HH
