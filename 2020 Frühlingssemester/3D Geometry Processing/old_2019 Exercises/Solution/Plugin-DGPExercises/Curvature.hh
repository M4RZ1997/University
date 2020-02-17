
#ifndef OPENFLIPPER_CURVATURE_HH
#define OPENFLIPPER_CURVATURE_HH

#include <ACG/Utils/ColorCoder.hh>

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

class Curvature {
public:
    using Point = ACG::Vec3d;

    Curvature(TriMesh& _mesh): mesh_(_mesh) {
        //set vertex_normal_ to default value
        mesh_.add_property(vertex_normal_, "temp normal");
        for(auto vh : mesh_.vertices())
            mesh_.property(vertex_normal_, vh) = mesh_.calc_vertex_normal(vh);

        //set vertex_curvature_ value
        if(!mesh_.get_property_handle(vertex_curvature_, "temp curvature"))
            mesh_.add_property(vertex_curvature_, "temp curvature");
        for(auto vh : mesh_.vertices())
            mesh_.property(vertex_curvature_, vh) = 0.;

        //add vertex valence
        mesh_.add_property(vertex_valence_, "vertex valence");

        //set weight properties
        mesh_.add_property(vertex_weight_, "vertex weights");
        for(auto vh : mesh_.vertices())
            mesh_.property(vertex_curvature_, vh) = 0.;

        if(!mesh_.get_property_handle(edge_weight_, "edge weights"))
            mesh_.add_property(edge_weight_, "edge weights");
        for(auto eh : mesh_.edges())
            mesh_.property(edge_weight_, eh) = 0.;

        //get average edge length
        avr_e_length_ = 0.;
        for(auto eh : mesh_.edges())
            avr_e_length_ += mesh_.calc_edge_length(eh);
        avr_e_length_ /= mesh_.n_edges();

        //initialize
        min_curvature_ = DBL_MAX;
        max_curvature_ = -DBL_MAX;
    }

    ~Curvature() {
        mesh_.remove_property(vertex_weight_);
//        mesh_.remove_property(edge_weight_);
        mesh_.remove_property(vertex_normal_);
//        mesh_.remove_property(vertex_curvature_);
        mesh_.remove_property(vertex_valence_);
    }

public:
    void show_normal(const int _normal_type);

    void show_curvature(const int _curvature_type);

    void show_valence();

    //-----------------------------------------------------//
    void compute_normals_with_constant_weights();

    void compute_normals_by_area_weights();

    void compute_normals_with_angle_weights();

    void calc_uniform_laplacian();

    void calc_mean_curvature();

    void calc_gauss_curvature();

    void calc_vertices_weights();

    void calc_edges_weights();

    void calc_weights();

    void update_normals();

    template <typename T>
    void color_coding(const OpenMesh::VPropHandleT<T>& _vprop, const double _min_value = 0.0,
            const double _max_value = 0.0, const int _bound = 20);

    double average_edge_length() const { return this->avr_e_length_; }

private:
    TriMesh& mesh_;
    OpenMesh::VPropHandleT<Point> vertex_normal_;
    OpenMesh::VPropHandleT<double> vertex_curvature_;
    OpenMesh::VPropHandleT<int> vertex_valence_;

    OpenMesh::VPropHandleT<double> vertex_weight_;
    OpenMesh::EPropHandleT<double> edge_weight_;
    double avr_e_length_;
    double min_curvature_, max_curvature_;
    ACG::ColorCoder color_;
};

#endif //OPENFLIPPER_CURVATURE_HH
