#ifndef OPENFLIPPER_PARAMETERIZATION_HH
#define OPENFLIPPER_PARAMETERIZATION_HH

#include <ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh>
#include "./eigen3/Eigen/Sparse"
#include "Curvature.hh"

class Parameterization : public Curvature {
public:
    Parameterization(TriMesh& _mesh) :
    Curvature(_mesh), mesh_(_mesh) {
        //get edge_weight_ property handle
        mesh_.get_property_handle(edge_weight_, "edge weights");
    }

    ~Parameterization(){}

public:
    void solve_harmonic_function(const std::vector<int>& _constraint_indices, const std::string& _property_name);

    void compute_isolines(TetrahedralMesh& _line_mesh, const std::vector<int>& _constraint_indices,
                          const std::string& _property_name, int _num_intervals);

private:
    std::pair<int, int> get_intervals_borders(const double _iso0, const double _iso1,
            const double _lower_bound, const double _interval_size);

    void add_isoline_segment(const std::pair<int, int>& _borders01, const std::pair<int, int>& _borders02,
            const double _iso0, const double _iso1, const double _iso2, const Point& _p0, const Point& _p1, const Point& _p2,
            double _lower_bound, double _interval_size);



private:
    TriMesh& mesh_;
    OpenMesh::EPropHandleT<double> edge_weight_;
    std::vector<Point> segment_points_;
};

#endif //OPENFLIPPER_PARAMETERIZATION_HH
