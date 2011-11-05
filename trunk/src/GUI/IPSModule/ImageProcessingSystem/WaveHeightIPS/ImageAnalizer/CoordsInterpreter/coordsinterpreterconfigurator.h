#ifndef COORDSINTERPRETERCONFIGURATOR_H
#define COORDSINTERPRETERCONFIGURATOR_H

#include <QtGui/QWidget>
#include "ui_coordsinterpreterconfigurator.h"

class CoordsInterpreterConfigurator : public QWidget
{
    Q_OBJECT

public:
    CoordsInterpreterConfigurator(QWidget *parent = 0);
    ~CoordsInterpreterConfigurator();

private:
    Ui::CoordsInterpreterConfiguratorClass ui;
};

#endif // COORDSINTERPRETERCONFIGURATOR_H
