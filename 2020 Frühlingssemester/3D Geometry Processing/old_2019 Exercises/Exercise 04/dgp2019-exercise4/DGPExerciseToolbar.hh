#pragma once

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "ui_DGPExerciseToolbarBase.h"


class DGPExerciseToolbar : public QWidget, public Ui::DGPExerciseToolbarBase
{
    Q_OBJECT

public:
    DGPExerciseToolbar(QWidget * parent = 0);
};