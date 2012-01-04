#ifndef GUIFRAMEWORK_H
#define GUIFRAMEWORK_H

#include <QtGui/QWidget>
#include "ui_guiframework.h"

class GUIFramework : public QWidget
{
    Q_OBJECT

public:
    GUIFramework(QWidget *parent = 0);
    ~GUIFramework();

private:
    Ui::GUIFrameworkClass ui;
};

#endif // GUIFRAMEWORK_H
