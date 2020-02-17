#ifndef OPENFLIPPER_CURVESMOOTHING_HH
#define OPENFLIPPER_CURVESMOOTHING_HH

#include <ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh>

class CurveSmoothing {
public:
    using Point = ACG::Vec3d;

    CurveSmoothing(TetrahedralMesh& _mesh): mesh_(_mesh) {}
    ~CurveSmoothing(){}

public:
    void generate_curve(const int _num_vertices);

    void laplacian_smoothing(const int _num_iter, const double _epsilon);

    void osculating_circle(const int _num_iter, const double _epsilon);


private:
    TetrahedralMesh& mesh_;
};
#endif //OPENFLIPPER_CURVESMOOTHING_HH
