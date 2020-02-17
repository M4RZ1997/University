#include "DGPExercisePlugin.hh"
#include "EigenTutorial.hh"
#include "IsoContouring.hh"
#include "CurveSmoothing.hh"
#include "Curvature.hh"
#include "PossionReconstructionT.hh"
#include "DelaunayTriangulation2D.hh"
#include "Smoothing.hh"
#include "Remeshing.hh"
#include "Parameterization.hh"
#include "ParameterizationII.hh"



void DGPExercisePlugin::initializePlugin()
{

    tool_ = new DGPExerciseToolbar();
    QSize size(300,300);
    tool_->resize(size);

    connect(tool_->solve_sls_pb, SIGNAL(clicked()), this, SLOT(slot_solve_sparse_linear_system()));
    connect(tool_->show_iso_pb, SIGNAL(clicked()), this, SLOT(slot_show_isovalue_and_level_set()));
    connect(tool_->gen_curve_pb, SIGNAL(clicked()), this, SLOT(slot_generate_curve()));
    connect(tool_->lpl_smoothing_pb, SIGNAL(clicked()), this, SLOT(slot_laplacian_smoothing()));
    connect(tool_->osc_smoothing_pb, SIGNAL(clicked()), this, SLOT(slot_osculating_circle()));
    connect(tool_->show_normal_pb, SIGNAL(clicked()), this, SLOT(slot_show_normal()));
    connect(tool_->show_curvature_pb, SIGNAL(clicked()), this, SLOT(slot_show_curvature()));
    connect(tool_->show_valence_pb, SIGNAL(clicked()), this, SLOT(slot_show_valence()));
    connect(tool_->poisson_reconstruction_pb, SIGNAL(clicked()), this, SLOT(slot_poisson_reconstruction()));
    connect(tool_->create_initial_mesh_pb, SIGNAL(clicked()), this, SLOT(slot_create_initial_mesh()));
    connect(tool_->set_2DView_pb, SIGNAL(clicked()), this, SLOT(slot_set_2DView()));
    connect(tool_->smooth_pb, SIGNAL(clicked()), this, SLOT(slot_smoothing()));
    connect(tool_->feature_enhance_pb, SIGNAL(clicked()), this, SLOT(slot_feature_enhance()));
    connect(tool_->remesh_pb, SIGNAL(clicked()), this, SLOT(slot_remeshing()));
    connect(tool_->tan_smooth_pb, SIGNAL(clicked()), this, SLOT(slot_tangential_smoothing()));
    connect(tool_->show_constraint_vhs_pb, SIGNAL(clicked()), this, SLOT(slot_show_constraint_vertices()));
    connect(tool_->solve_harmonics_pb, SIGNAL(clicked()), this, SLOT(slot_solve_harmonic_functions()));
    connect(tool_->computer_iso_lines_pb, SIGNAL(clicked()), this, SLOT(slot_compute_isolines()));
    connect(tool_->map_boundary_pb, SIGNAL(clicked()), this, SLOT(slot_map_boundary_to_circle()));
    connect(tool_->exp_smooth_tex_pb, SIGNAL(clicked()), this, SLOT(slot_explicitly_smooth_texture()));
    connect(tool_->imp_smooth_tex_pb, SIGNAL(clicked()), this, SLOT(slot_implicitly_smooth_texture()));
    connect(tool_->compute_minimum_surface_pb, SIGNAL(clicked()), this, SLOT(slot_compute_minimum_surface()));

    emit addToolbox( tr("DGPExercises") , tool_ );
}


void DGPExercisePlugin::pluginsInitialized()
{
    constraint_vhs_.push_back(40);
    constraint_vhs_.push_back(484);


    // Define a new picking mode
    emit addPickMode("Separator");
    emit addPickMode("DGP Delaunay2D");
    emit addPickMode("DGP Parameterization");

}

//====================================================================================================================//
void DGPExercisePlugin::slot_solve_sparse_linear_system()
{
    EigenTutorial et;
    et.solve_sparse_linear_system();
}

//====================================================================================================================//
void DGPExercisePlugin::slot_show_isovalue_and_level_set() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        TriMeshObject *tri_obj = PluginFunctions::triMeshObject(*o_it);
        TriMesh *trimesh = tri_obj->mesh();

        if (trimesh) {
            //create a level set object
            int line_obj_id;
            emit addEmptyObject(DATA_TETRAHEDRAL_MESH, line_obj_id);
            auto *line_obj = PluginFunctions::tetrahedralMeshObject(line_obj_id);
            line_obj->setName("LevelSet");
            auto *line_mesh = line_obj->mesh();

            //compute isovalue, color vertices and create a level set
            IsoContouring ic(*trimesh, *line_mesh, tool_->function_cb->currentIndex());
            ic.show_isovalue_and_level_set();

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);
            line_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::addDrawMode("Edges (wireframe)"));

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}

//====================================================================================================================//
void DGPExercisePlugin::slot_generate_curve() {
    //create a curve object
    int curve_obj_id;
    emit addEmptyObject(DATA_TETRAHEDRAL_MESH, curve_obj_id);
    auto *curve_obj =PluginFunctions::tetrahedralMeshObject(curve_obj_id);
    curve_obj->setName("Curve");
    curve_obj->materialNode()->set_point_size(6.0);
    auto *curve = curve_obj->mesh();

    CurveSmoothing cs(*curve);
    cs.generate_curve(tool_->num_vertices_sb->value());

    auto d0 = ACG::SceneGraph::DrawModes::addDrawMode("Edges (wireframe)");
    auto d1 = ACG::SceneGraph::DrawModes::addDrawMode("Vertices");
    d0 |= d1;
    curve_obj->meshNode()->drawMode(d0);

    emit updatedObject(curve_obj->id(), UPDATE_ALL);
}


void DGPExercisePlugin::slot_laplacian_smoothing() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TETRAHEDRAL_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto* curve = PluginFunctions::tetrahedralMesh(*o_it);
        if(curve) {
            CurveSmoothing cs(*curve);
            cs.laplacian_smoothing(tool_->num_iter_sb->value(), tool_->epsilon_dsb->value());

            emit updatedObject(o_it->id(), UPDATE_ALL);
        }
    }
}


void DGPExercisePlugin::slot_osculating_circle() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TETRAHEDRAL_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto* curve = PluginFunctions::tetrahedralMesh(*o_it);
        if(curve) {
            CurveSmoothing cs(*curve);
            cs.osculating_circle(tool_->num_iter_sb->value(), tool_->epsilon_dsb->value());

            emit updatedObject(o_it->id(), UPDATE_ALL);
        }
    }
}

//====================================================================================================================//
void DGPExercisePlugin::slot_show_normal() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        TriMeshObject *tri_obj = PluginFunctions::triMeshObject(*o_it);
        TriMesh *trimesh = tri_obj->mesh();

        if (trimesh) {
                for(auto fh : trimesh->faces())
                    trimesh->set_color(fh, ACG::Vec4f(0.2, 0.7, 0.4, 1.0));

            Curvature curvature(*trimesh);
            curvature.show_normal(tool_->normal_type_cb->currentIndex());

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_SMOOTH_SHADED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}

void DGPExercisePlugin::slot_show_curvature() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        TriMeshObject *tri_obj = PluginFunctions::triMeshObject(*o_it);
        TriMesh *trimesh = tri_obj->mesh();

        if (trimesh) {
            Curvature curvature(*trimesh);
            curvature.show_curvature(tool_->curvature_type_cb->currentIndex());

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME
            | ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}

void DGPExercisePlugin::slot_show_valence() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        TriMeshObject *tri_obj = PluginFunctions::triMeshObject(*o_it);
        TriMesh *trimesh = tri_obj->mesh();

        if (trimesh) {
            Curvature curvature(*trimesh);
            curvature.show_valence();

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME
                                          | ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}

//====================================================================================================================//
void DGPExercisePlugin::slot_poisson_reconstruction() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            // Add empty triangle mesh
            int new_obj_id;
            emit addEmptyObject(DATA_TRIANGLE_MESH, new_obj_id);
            auto new_obj = PluginFunctions::triMeshObject(new_obj_id);

            // Get pointer to triangle mesh
            auto new_mesh = new_obj->mesh();

            //Reconstruct
            PoissonReconstructionT<TriMesh> pr;

            PoissonReconstructionT<TriMesh>::Parameter params;
            params.Depth = tool_->octree_depth_sb->value();

            if (pr.run(*trimesh, *new_mesh, params)) {
                emit log(LOGINFO, "Reconstruction succeeded");
                emit updatedObject(new_obj_id, UPDATE_ALL);
                new_obj->setName("Poisson Reconstruction.obj");
                new_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::SOLID_FLAT_SHADED);
            }
        }
    }
}

//====================================================================================================================//
void DGPExercisePlugin::slotMouseEvent(QMouseEvent* _event) {
    // control modifier is reserved for selecting target
    if (_event->modifiers() & (Qt::ControlModifier)) {
        return;
    }
    if(_event->type() != QEvent::MouseButtonPress) {
        return;
    }
    if (PluginFunctions::pickMode()   == ("DGP Delaunay2D") &&
        PluginFunctions::actionMode() == Viewer::PickingMode) {

        // handle mouse events
        if (_event->button() == Qt::LeftButton) {
            size_t node_idx, target_idx;
            ACG::Vec3d hit_point;

            // first, check if we picked an edge
            if (PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_EDGE, _event->pos(),
                    node_idx, target_idx, &hit_point)) {
                BaseObjectData *obj;
                if (PluginFunctions::getPickedObject(node_idx, obj)) {
                    // is picked object a triangle mesh?
                    auto tri_obj = PluginFunctions::triMeshObject(obj);

                    if (tri_obj) {
                        auto eh = tri_obj->mesh()->edge_handle(target_idx);
                        if (eh == TriMesh::InvalidEdgeHandle)
                            return;

                        // insert the respective point into the triangulation
                        insert_point(tri_obj, true, TriMesh::InvalidFaceHandle, eh, hit_point, hcolors_.get_next_color());
                        emit updatedObject(tri_obj->id(), UPDATE_ALL);

                        return;
                    }
                }
            }// end of scenegraph edge picking

            // if we haven't picked an edge, pick a face
            if (PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_FACE, _event->pos(),
                    node_idx, target_idx, &hit_point)) {
                BaseObjectData *obj;
                if (PluginFunctions::getPickedObject(node_idx, obj)) {
                    // is picked object a triangle mesh?
                    TriMeshObject *tri_obj = PluginFunctions::triMeshObject(obj);

                    if (tri_obj) {
                        auto fh = tri_obj->mesh()->face_handle(target_idx);
                        if (fh == TriMesh::InvalidFaceHandle)
                            return;
                        // insert the respective point into the triangulation
                        insert_point(tri_obj, false, fh, TriMesh::InvalidEdgeHandle, hit_point, hcolors_.get_next_color());
                        emit updatedObject(tri_obj->id(), UPDATE_ALL);

                        return;
                    }
                }
            } // end of scenegraph face picking

        } // end of if left button
    }

    if (PluginFunctions::pickMode()   == ("DGP Parameterization") &&
        PluginFunctions::actionMode() == Viewer::PickingMode) {
        // handle mouse events
        if (_event->button() == Qt::LeftButton) {
            size_t node_idx, target_idx;
            ACG::Vec3d hit_point;
            // pick vertices
            if (PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_VERTEX, _event->pos(),
                                                node_idx, target_idx, &hit_point)) {
                BaseObjectData *obj;
                if (PluginFunctions::getPickedObject(node_idx, obj)) {
                    // is picked object a triangle mesh?
                    TriMeshObject *tri_obj = PluginFunctions::triMeshObject(obj);

                    if (tri_obj) {
                        auto vh = tri_obj->mesh()->vertex_handle(target_idx);
                        if (vh == TriMesh::InvalidVertexHandle)
                            return;
                        //set constraint vertex
                        for(int i=0; i<2; ++i)
                            if(tool_->vertex_number_cb->currentIndex() == i)
                                constraint_vhs_[i] = vh.idx();

                        slot_show_constraint_vertices();

                        return;
                    }
                }
            } // end of scenegraph face picking
        }
    }

    emit updateView();
}

void DGPExercisePlugin::slot_create_initial_mesh() {
    // create an inital mesh that consists of two triangles
    int id;
    emit addEmptyObject(DATA_TRIANGLE_MESH , id);
    auto tri_obj = PluginFunctions::triMeshObject(id);

    add_initial_mesh(tri_obj, hcolors_);

    emit updatedObject(tri_obj->id(), UPDATE_ALL);

    // set the pick mode to 2D delaunay mode
    PluginFunctions::pickMode("DGP Delaunay2D");

    // set 2d view
    slot_set_2DView();
}


void DGPExercisePlugin::slot_set_2DView() {
    // set orthographic projection
    PluginFunctions::orthographicProjection();
    // set viewing direction to z axis
    PluginFunctions::viewingDirection(-0.35 * ACG::Vec3d(0.0, 0.0, 1.0), ACG::Vec3d(0.0, 1.0, 0.0));
}


//====================================================================================================================//
void DGPExercisePlugin::slot_smoothing() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            Smoothing smoother(*trimesh);
            if(tool_->method_cb->currentIndex() == 0)
                smoother.uniform_smooth(tool_->iter_sb->value());
            else if(tool_->method_cb->currentIndex() == 1)
                smoother.cotan_laplacian_smooth(tool_->iter_sb->value());
            else
                smoother.implicit_smooth(tool_->time_step_dsb->value());

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}

void DGPExercisePlugin::slot_feature_enhance() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            Smoothing smoother(*trimesh);
            if(tool_->method_cb->currentIndex() == 0)
                smoother.uniform_laplacian_enhance_feature(tool_->iter_sb->value(), tool_->coefficient_dsb->value());
            else if(tool_->method_cb->currentIndex() == 1)
                smoother.cotan_laplacian_enhance_feature(tool_->iter_sb->value(), tool_->coefficient_dsb->value());

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}


//====================================================================================================================//
void DGPExercisePlugin::slot_remeshing() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            Remeshing remesher(*trimesh);
            remesher.remesh(tool_->remesh_type_cb->currentIndex(), tool_->remesh_iter_sb->value(),
                    tool_->target_length_ratio_dsb->value());

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME | ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}

void DGPExercisePlugin::slot_tangential_smoothing() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            Remeshing remesher(*trimesh);
            remesher.tangential_relaxation();

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME | ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}


//======================================================================================================================
void DGPExercisePlugin::slot_show_constraint_vertices() {
    // set the pick mode to 2D delaunay mode
    PluginFunctions::pickMode("DGP Parameterization");

    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        tri_obj->materialNode()->set_point_size(12);

        for(auto vh : trimesh->vertices()) {
            if(constraint_vhs_[0] == vh.idx())
                trimesh->set_color(vh, ACG::Vec4f(1,0,0,1));
            else if(constraint_vhs_[1] == vh.idx())
                trimesh->set_color(vh, ACG::Vec4f(0,0,1,1));
            else
                trimesh->set_color(vh, ACG::Vec4f(1,1,1,0));
        }

        tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME
        | ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED | ACG::SceneGraph::DrawModes::POINTS_COLORED);

        tri_obj->materialNode()->enable_alpha_test(0.8);

        emit updatedObject(tri_obj->id(), UPDATE_COLOR);
    }
}

void DGPExercisePlugin::slot_solve_harmonic_functions() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            Parameterization pmr(*trimesh);
            std::vector<int> constraint_vhs0{constraint_vhs_[0], constraint_vhs_[1]};
            pmr.solve_harmonic_function(constraint_vhs0, "harmonic functions0");

            //set draw mode
            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME
                                          | ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
        }
    }
}

void DGPExercisePlugin::slot_compute_isolines() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            Parameterization pmr(*trimesh);
            std::vector<int> constraint_vhs0{constraint_vhs_[0], constraint_vhs_[1]};

            // Add empty triangle mesh
            int iso_line_obj0, iso_line_obj1;
            emit addEmptyObject(DATA_TETRAHEDRAL_MESH, iso_line_obj0);
            auto mesh_obj0 = PluginFunctions::tetrahedralMeshObject(iso_line_obj0);
            mesh_obj0->materialNode()->set_line_width(5);
            mesh_obj0->setName("Iso Curves");

            pmr.compute_isolines(*mesh_obj0->mesh(), constraint_vhs0, "harmonic functions0", tool_->num_intervals_sb->value());


            //set draw mode
            mesh_obj0->meshNode()->drawMode(ACG::SceneGraph::DrawModes::addDrawMode("Edges (wireframe)"));


            emit updatedObject(mesh_obj0->id(), UPDATE_ALL);
            break;
        }
    }
}


//======================================================================================================================
void DGPExercisePlugin::slot_map_boundary_to_circle() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            ACG::Vec3d min, max;
            tri_obj->boundingBox(min, max);
            auto radius = (max - min).norm();
            auto origin = ACG::Vec3d((min[0]+max[0])/2., (min[1]+max[1])/2., min[2]);
            // Add empty triangle mesh
            int tex_obj_id;
            emit addEmptyObject(DATA_TRIANGLE_MESH, tex_obj_id);
            auto tex_obj = PluginFunctions::triMeshObject(tex_obj_id);
            tex_obj->setName("Texture");
            tex_obj->target(true);

            ParameterizationII pmr(*trimesh, *tex_obj->mesh());
            pmr.map_suface_boundary_to_circle(origin, radius/20.);

            //set draw mode
            tex_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME);

            emit updatedObject(tex_obj_id, UPDATE_ALL);

            break;
        }
    }
}

void DGPExercisePlugin::slot_explicitly_smooth_texture() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            if(++o_it == PluginFunctions::objectsEnd()) {
                std::cerr<<"\nMap the boundary to a circle first!";
                return;
            }
            auto tex_obj = PluginFunctions::triMeshObject(*o_it);
            if(!tex_obj){
                std::cerr<<"\nMap the boundary to a circle first!";
                return;
            }
            if(tex_obj->name() == "Texture") {
                ACG::Vec3d min, max;
                tri_obj->boundingBox(min, max);
                auto origin = ACG::Vec3d((min[0]+max[0])/2., (min[1]+max[1])/2., min[2]);

                ParameterizationII pmr(*trimesh, *tex_obj->mesh());
                pmr.explicitly_smooth_texture(origin, tool_->exp_texture_smooth_iter_sb->value());

                //set texture
//                QImage image(256, 256, QImage::Format_RGB32);
//                int value;
//                for (int row = 0; row < 256; row++) {
//                    for (int col = 0; col < 256; col++) {
//                        // Each cell is 8x8, value is 0 or 255 (black or white)
//                        value = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
//                        image.setPixelColor(row, col, QColor(value, value, value, 1));
//                    }
//                }
                QImage image;
                QString filePath = OpenFlipper::Options::textureDirStr() + QDir::separator() + "UV_black.png";
                image.load(filePath);
                tri_obj->textureNode()->set_texture(image);
                tri_obj->setObjectDrawMode(ACG::SceneGraph::DrawModes::SOLID_TEXTURED_SHADED, PluginFunctions::ALL_VIEWERS);

                emit updatedObject(tri_obj->id(), UPDATE_ALL);
                emit updatedObject(tex_obj->id(), UPDATE_GEOMETRY);

                break;
            }
        }
    }
}

void DGPExercisePlugin::slot_implicitly_smooth_texture() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            if(++o_it == PluginFunctions::objectsEnd()) {
                std::cerr<<"\nMap the boundary to a circle first!";
                return;
            }
            auto tex_obj = PluginFunctions::triMeshObject(*o_it);
            if(!tex_obj){
                std::cerr<<"\nMap the boundary to a circle first!";
                return;
            }
            if(tex_obj->name() == "Texture") {
                ACG::Vec3d min, max;
                tri_obj->boundingBox(min, max);
                auto origin = ACG::Vec3d((min[0]+max[0])/2., (min[1]+max[1])/2., min[2]);

                ParameterizationII pmr(*trimesh, *tex_obj->mesh());
                pmr.implicitly_smooth_texture(origin);

                //set texture
                QImage image;
                QString filePath = OpenFlipper::Options::textureDirStr() + QDir::separator() + "UV_black.png";
                image.load(filePath);
                tri_obj->textureNode()->set_texture(image);
                tri_obj->setObjectDrawMode(ACG::SceneGraph::DrawModes::SOLID_TEXTURED_SHADED, PluginFunctions::ALL_VIEWERS);

                emit updatedObject(tri_obj->id(), UPDATE_ALL);
                emit updatedObject(tex_obj->id(), UPDATE_GEOMETRY);

                break;
            }
        }
    }
}

void DGPExercisePlugin::slot_compute_minimum_surface() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        auto tri_obj = PluginFunctions::triMeshObject(*o_it);
        auto trimesh = tri_obj->mesh();

        if (trimesh) {
            TriMesh tex_mesh;
            ParameterizationII pmr(*trimesh, tex_mesh);
            pmr.minimal_surface();

            tri_obj->meshNode()->drawMode(ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED);

            emit updatedObject(tri_obj->id(), UPDATE_ALL);
            break;
        }
    }
}

//======================================================================================================================
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(dgpexerciseplugin, DGPExercisePlugin);
#endif