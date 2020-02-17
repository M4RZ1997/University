#include "DGPExercisePlugin.hh"

#include "EigenTutorial.hh"
#include "IsoContouring.hh"

void DGPExercisePlugin::initializePlugin()
{

    tool_ = new DGPExerciseToolbar();
    QSize size(300,300);
    tool_->resize(size);

    connect(tool_->solve_sls_pb, SIGNAL(clicked()), this, SLOT(slot_solve_sparse_linear_system()));
    connect(tool_->show_iso_pb, SIGNAL(clicked()), this, SLOT(slot_show_isovalue_and_level_set()));

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


void DGPExercisePlugin::slot_show_isovalue_and_level_set()
{
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DATA_TRIANGLE_MESH) ; o_it != PluginFunctions::objectsEnd(); ++o_it) {
        TriMeshObject* tri_obj= PluginFunctions::triMeshObject(*o_it);
        TriMesh *trimesh = tri_obj->mesh();

        if(trimesh) {
            //create a level set object
            int line_obj_id;
            emit addEmptyObject(DATA_TETRAHEDRAL_MESH, line_obj_id);
            auto *line_obj =PluginFunctions::tetrahedralMeshObject(line_obj_id);
            line_obj->setName("LevelSet");
//            line_obj->show();
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

//======================================================================================================================
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(tetctplugin, TetCTPlugin);
#endif