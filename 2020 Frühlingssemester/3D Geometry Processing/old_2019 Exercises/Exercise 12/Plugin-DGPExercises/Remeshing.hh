#ifndef OPENFLIPPER_REMESHING_HH
#define OPENFLIPPER_REMESHING_HH

#include <queue>
#include "Curvature.hh"

class Remeshing : public Curvature {
public:
    using PairLE = std::pair<double, OpenMesh::EdgeHandle>;
    using PairLHE = std::pair<double, OpenMesh::HalfedgeHandle>;


public:
    Remeshing(TriMesh& _mesh) :
            Curvature(_mesh), mesh_(_mesh) {
        //get vertex_weight_ property handle
        mesh_.get_property_handle(vertex_weight_, "vertex weights");
        //get edge_weight_ property handle
        mesh_.get_property_handle(edge_weight_, "edge weights");
        //get temp normal property
        mesh_.get_property_handle(vertex_normal_, "temp normal");
        for(auto vh : mesh_.vertices())
            mesh_.property(vertex_normal_, vh) = mesh_.calc_vertex_normal(vh);
        //get temp curvature
        mesh_.get_property_handle(vertex_curvature_, "temp curvature");

        //add curvature properties
        mesh_.add_property(uni_curvature_, "uniform curvature");
        mesh_.add_property(mean_curvature_, "mean curvature");
        mesh_.add_property(gauss_curvature_, "gauss curvature");

        //add target edge length property
        mesh_.add_property(target_length_, "target edge length");
        //add edge length property
        mesh_.add_property(edge_length_, "edge length");
        for(auto eh : mesh_.edges())
            mesh_.property(edge_length_, eh) = mesh_.calc_edge_length(eh);
    }

    ~Remeshing() {
        mesh_.remove_property(uni_curvature_);
        mesh_.remove_property(mean_curvature_);
        mesh_.remove_property(gauss_curvature_);
        mesh_.remove_property(target_length_);
        mesh_.remove_property(edge_length_);
    }

    void remesh(const int _remeshing_type, const int _num_iterations, const int _target_length_ratio);

    void tangential_relaxation();

private:
    void calc_target_length(const int _remeshing_type, const int _target_length_ratio);

    void split_long_edges();

    void collapse_short_edges();

    void equalize_valences();



private:
    TriMesh& mesh_;
    OpenMesh::VPropHandleT<double> vertex_weight_;
    OpenMesh::EPropHandleT<double> edge_weight_;
    OpenMesh::VPropHandleT<Point> vertex_normal_;
    OpenMesh::VPropHandleT<double> vertex_curvature_;

    OpenMesh::VPropHandleT<double> uni_curvature_;
    OpenMesh::VPropHandleT<double> mean_curvature_;
    OpenMesh::VPropHandleT<double> gauss_curvature_;

    OpenMesh::VPropHandleT<double> target_length_;
    OpenMesh::EPropHandleT<double> edge_length_;
};

#endif //OPENFLIPPER_REMESHING_HH
