#ifndef COORDSINTERPRETERCONFIGURATOR_H
#define COORDSINTERPRETERCONFIGURATOR_H

#include <QtGui/QWidget>
#include <qdialog.h>
#include "ui_coordsinterpreterconfigurator.h"

#include "GUIConfiguratorDialog.h"

class CoordsInterpreterConfigurator : public GUIConfiguratorDialog
{
    Q_OBJECT

public:
    CoordsInterpreterConfigurator(QWidget *parent = 0);
    ~CoordsInterpreterConfigurator();

private:
    Ui::CoordsInterpreterConfiguratorClass ui;
};

#endif // COORDSINTERPRETERCONFIGURATOR_H
