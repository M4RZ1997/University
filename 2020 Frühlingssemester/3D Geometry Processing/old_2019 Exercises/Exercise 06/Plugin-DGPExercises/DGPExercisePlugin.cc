#include "DGPExercisePlugin.hh"

#include "EigenTutorial.hh"
#include "IsoContouring.hh"
#include "CurveSmoothing.hh"
#include "Curvature.hh"
#include "PossionReconstructionT.hh"

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
    connect(tool_->poisson_reconstruction_pb, SIGNAL(clicked()), this, SLOT(slot_poisson_reconstruction()));


    emit addToolbox( tr("DGPExercise") , tool_ );
}


void DGPExercisePlugin::pluginsInitialized()
{
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

//====================================================================================================================//
void DGPExercisePlugin::slot_poisson_reconstruction() {
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH);
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        TriMeshObject *tri_obj = PluginFunctions::triMeshObject(*o_it);
        TriMesh *trimesh = tri_obj->mesh();

        if (trimesh) {
            // Add empty triangle mesh
            int new_obj_id;
            emit addEmptyObject(DATA_TRIANGLE_MESH, new_obj_id);
            auto *new_obj =PluginFunctions::triMeshObject(new_obj_id);

            // Get pointer to triangle mesh
            TriMesh* new_mesh = new_obj->mesh();

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

//======================================================================================================================
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(dgpexerciseplugin, DGPExercisePlugin);
#endif