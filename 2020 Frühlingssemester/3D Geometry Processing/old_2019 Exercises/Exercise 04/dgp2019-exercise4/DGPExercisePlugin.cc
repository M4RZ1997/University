#include "DGPExercisePlugin.hh"

#include "EigenTutorial.hh"
#include "IsoContouring.hh"
#include "CurveSmoothing.hh"

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


    emit addToolbox( tr("DGPExercise") , tool_ );
}


void DGPExercisePlugin::pluginsInitialized()
{
}


void DGPExercisePlugin::slot_solve_sparse_linear_system()
{
    EigenTutorial et;
    et.solve_sparse_linear_system();
}


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


//======================================================================================================================
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(dgpexerciseplugin, DGPExercisePlugin);
#endif