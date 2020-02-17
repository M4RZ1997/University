#ifndef OPENFLIPPER_DGPEXERCISEPLUGIN_HH
#define OPENFLIPPER_DGPEXERCISEPLUGIN_HH

#include <QObject>

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <ACG/Utils/HaltonColors.hh>
#include <ACG/Scenegraph/LineNode.hh>

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include "DGPExerciseToolbar.hh"



class DGPExercisePlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, LoadSaveInterface, MouseInterface, PickingInterface
{
Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(ToolboxInterface)
    Q_INTERFACES(LoggingInterface)
    Q_INTERFACES(LoadSaveInterface)
    Q_INTERFACES(MouseInterface)
    Q_INTERFACES(PickingInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-DGPExercise")
#endif

signals:
    void updateView();

    //LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // ToolboxInterface
    void addToolbox(QString _name, QWidget* _widget);

    // LoadSaveInterface
    void addEmptyObject(DataType _type, int& _id);
    void updatedObject(int _id, const UpdateType& _type);

    // PickingInterface
    void addPickMode(const std::string &_mode);


private slots:
    // initialization functions
    void initializePlugin();
    void pluginsInitialized();

    void slotMouseEvent(QMouseEvent* _event);


public :
    ~DGPExercisePlugin() {}
    QString name() { return (QString("DGPExercisePlugin")); };
    QString description( ) { return (QString("Digital Geometry Processing exercise plugin")); };


public slots:
    void slot_solve_sparse_linear_system();

    void slot_show_isovalue_and_level_set();

    void slot_generate_curve();

    void slot_laplacian_smoothing();

    void slot_osculating_circle();

    void slot_show_normal();

    void slot_show_curvature();

    void slot_show_valence();

    void slot_poisson_reconstruction();

    void slot_create_initial_mesh();

    void slot_set_2DView();

    void slot_smoothing();

    void slot_feature_enhance();

    void slot_remeshing();

    void slot_tangential_smoothing();

    void slot_show_constraint_vertices();

    void slot_solve_harmonic_functions();

    void slot_compute_isolines();

    void slot_map_boundary_to_circle();

    void slot_explicitly_smooth_texture();

    void slot_implicitly_smooth_texture();

    void slot_compute_minimum_surface();

private:
    /// Widget for Toolbox
    DGPExerciseToolbar* tool_;

    ACG::HaltonColors hcolors_;

    //store selected vertices
    std::vector<int> constraint_vhs_;
};

#endif //OPENFLIPPER_DGPEXERCISEPLUGIN_HH
