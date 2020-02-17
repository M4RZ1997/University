#ifndef OPENFLIPPER_ISOCONTOURING_HH
#define OPENFLIPPER_ISOCONTOURING_HH

#include <ACG/Utils/ColorCoder.hh>

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh>

class IsoContouring {
public:
    using Point = ACG::Vec3d;

    IsoContouring(TriMesh& _trimesh, TetrahedralMesh& _line_mesh, const int _function_id):
    trimesh_(_trimesh),
    line_mesh_(_line_mesh),
    function_id_(_function_id)
    {}
    ~IsoContouring(){}

public:
    void show_isovalue_and_level_set();

private:
    double iso_value(const Point& _pt) const;

    void show_isovalue();

    void compute_segment_points();

    void create_level_set0_segments();

private:
    TriMesh& trimesh_;
    TetrahedralMesh& line_mesh_;
    const int function_id_;
    std::vector<Point> segment_points_;
    ACG::ColorCoder color_;
};

#endif //OPENFLIPPER_ISOCONTOURING_HH
