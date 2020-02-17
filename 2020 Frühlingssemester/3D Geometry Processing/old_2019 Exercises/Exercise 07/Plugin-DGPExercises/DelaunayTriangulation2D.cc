#include "DelaunayTriangulation2D.hh"
#include <queue>


void insert_point(TriMeshObject * _tri_obj, const bool _on_edge, const TriMesh::FaceHandle& _fh,
        const TriMesh::EdgeHandle& _eh, const TriMesh::Point& _p, const ACG::Vec4f& _color) {
    if (_tri_obj == 0)
        return;

    auto mesh = _tri_obj->mesh();

    // add vertex, assign random color to it
    auto p = TriMesh::Point(_p[0], _p[1], 0.0);
    auto vh = mesh->add_vertex(p);

    // add a cone to visualize the corresponding voronio cell
    auto node = get_GlutPrimitiveNode(_tri_obj);
    auto axis = ACG::Vec3d(0,0,1);

    // add an offset to the cones position, since the position is not located at the apex
    // but at the intersection of axis and base plate
    auto offset = ACG::Vec3d(0,0,-1);
    node->add_primitive(ACG::SceneGraph::GlutPrimitiveNode::CONE, p+offset, axis, _color);

    if (_on_edge && _eh != TriMesh::InvalidEdgeHandle) {
        mesh->split(_eh, vh);
        std::cout << "Split: vertex " << vh.idx() << " at position ";
        std::cout << p << " on edge " << _eh.idx() << std::endl;
    } else if (_fh != TriMesh::InvalidFaceHandle) {
        mesh->split(_fh, vh);
        std::cout << "Split: vertex " << vh.idx() << " at position ";
        std::cout << p << " inside triangle " << _fh.idx() << std::endl;
    } else {
        return;
    }

    // ============================================================================
    // Exercise 7: Delaunay Triangulation
    // The task is to re-establish Delaunay property
    // ... find edges opposite to the inserted vertex
    // ... are these edges ok? otherwise: flip'em
    // ============================================================================
    //----------Add your code here----------

    //--------------------------------------

    mesh->garbage_collection();
}

bool is_delaunay(TriMesh * _mesh, TriMesh::EdgeHandle _eh) {
    bool result = true;

    //----------Add your code here----------
    // is the edge _eh delaunay or not?
    // -> circum-circle test of the four points (a,b,c,d)


    //--------------------------------------
    return result;
}


void add_initial_mesh(TriMeshObject* _tri_obj, ACG::HaltonColors& _hcolors) {
    _tri_obj->target(true);
    _tri_obj->show();
    _tri_obj->setName("DelaunayMesh");
    _tri_obj->materialNode()->set_specular_color(ACG::Vec4f(0.0, 0.0, 0.0, 1.0));
    _tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME);

    auto mesh = _tri_obj->mesh();

    auto p0 = TriMesh::Point(0, 0, 0);
    auto p1 = TriMesh::Point(1, 0, 0);
    auto p2 = TriMesh::Point(1, 1, 0);
    auto p3 = TriMesh::Point(0, 1, 0);

    // add 4 vertices
    TriMesh::VertexHandle vh[4];
    vh[0] = mesh->add_vertex(p0);
    vh[1] = mesh->add_vertex(p1);
    vh[2] = mesh->add_vertex(p2);
    vh[3] = mesh->add_vertex(p3);

    // add cones
    auto node = get_GlutPrimitiveNode(_tri_obj);
    auto axis = ACG::Vec3d(0,0,1);
    // add an offset to the cones position, since the position is not located at the apex
    // but at the intersection of axis and base plate
    auto offset = ACG::Vec3d(0,0,-1);

    node->add_primitive(ACG::SceneGraph::GlutPrimitiveNode::CONE, p0+offset, axis, _hcolors.get_next_color());
    node->add_primitive(ACG::SceneGraph::GlutPrimitiveNode::CONE, p1+offset, axis, _hcolors.get_next_color());
    node->add_primitive(ACG::SceneGraph::GlutPrimitiveNode::CONE, p2+offset, axis, _hcolors.get_next_color());
    node->add_primitive(ACG::SceneGraph::GlutPrimitiveNode::CONE, p3+offset, axis, _hcolors.get_next_color());

    // add 2 initial triagles
    mesh->add_face(vh[0], vh[1], vh[2]);
    mesh->add_face(vh[0], vh[2], vh[3]);
}

ACG::SceneGraph::GlutPrimitiveNode*
get_GlutPrimitiveNode(TriMeshObject* _tri_obj) {
    // get or add material node for primitive node
    ACG::SceneGraph::MaterialNode * material_node = 0;
    if(!_tri_obj->hasAdditionalNode("DGPExercisesPlugin", "PrimitiveMaterialNode")) {
        material_node = new ACG::SceneGraph::MaterialNode(
                _tri_obj->manipulatorNode(), "DGP Delaunay2D: Cone Material");

        if(!_tri_obj->addAdditionalNode(
                material_node, QString("DGPExercisesPlugin"), "PrimitiveMaterialNode")
                ) {
            std::cerr << "DGPExercisesPlugin::get_GlutPrimitiveNode(): ";
            std::cerr << "could not add an primitive material node\n";
            return 0;
        }
    } else {
        _tri_obj->getAdditionalNode(material_node,  "DGPExercisesPlugin", "PrimitiveMaterialNode" );
    }

    material_node->set_specular_color(ACG::Vec4f(0.0, 0.0, 0.0, 1.0));

    ACG::SceneGraph::GlutPrimitiveNode * glut_node = 0;
    if(!_tri_obj->hasAdditionalNode("DGPExercisesPlugin", "GlutPrimitiveNode")) {
        glut_node = new ACG::SceneGraph::GlutPrimitiveNode(ACG::SceneGraph::GlutPrimitiveNode::CONE,
                                                           material_node, "DGP Delaunay2D: Cones");
        if(!_tri_obj->addAdditionalNode(glut_node, "DGPExercisesPlugin", "GlutPrimitiveNode")) {
            std::cerr << "DGPExercisesPlugin::get_GlutPrimitiveNode(): ";
            std::cerr << "could not add glut primitive node\n";
            return nullptr;
        }
        glut_node->clear();
        glut_node->drawMode(ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED);
        glut_node->show();
    } else {
        _tri_obj->getAdditionalNode(glut_node, "DGPExercisesPlugin", "GlutPrimitiveNode" );
    }

    return glut_node;
}