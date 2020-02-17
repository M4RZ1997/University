#ifndef OPENFLIPPER_DGPEXERCISEPLUGIN_HH
#define OPENFLIPPER_DGPEXERCISEPLUGIN_HH

#include <QObject>

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>

#include "DGPExerciseToolbar.hh"

class DGPExercisePlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, LoadSaveInterface
{
Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(ToolboxInterface)
    Q_INTERFACES(LoggingInterface)
    Q_INTERFACES(LoadSaveInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-DGPExercise")
#endif

signals:
    void updateView();

    //LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // ToolboxInterface
    void addToolbox( QString _name  , QWidget* _widget );

    // LoadSaveInterface
    void addEmptyObject( DataType _type, int& _id);
    void updatedObject(int _id, const UpdateType& _type);

private slots:
    // initialization functions
    void initializePlugin();
    void pluginsInitialized();

public :
    ~DGPExercisePlugin() {}
    QString name() { return (QString("DGPExercisePlugin")); };
    QString description( ) { return (QString("Digital Geometry Processing exercise plugin")); };


public slots:
    void slot_solve_sparse_linear_system();

    void slot_show_isovalue_and_level_set();

private:
    /// Widget for Toolbox
    DGPExerciseToolbar* tool_;
};

#endif //OPENFLIPPER_DGPEXERCISEPLUGIN_HH
