
#ifndef OPENFLIPPER_DELAUNAYTRIANGULATION2D_HH
#define OPENFLIPPER_DELAUNAYTRIANGULATION2D_HH

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ACG/Scenegraph/GlutPrimitiveNode.hh>
#include <ACG/Utils/HaltonColors.hh>


void insert_point(TriMeshObject * _tri_obj, const bool _on_edge, const TriMesh::FaceHandle& _fh,
                  const TriMesh::EdgeHandle& _eh, const TriMesh::Point& _p, const ACG::Vec4f& _color);

bool is_delaunay(TriMesh * _mesh, TriMesh::EdgeHandle _eh);

void add_initial_mesh(TriMeshObject* _tri_obj, ACG::HaltonColors& _hcolors);

ACG::SceneGraph::GlutPrimitiveNode* get_GlutPrimitiveNode(TriMeshObject* _tri_obj);


#endif //OPENFLIPPER_DELAUNAYTRIANGULATION2D_HH
