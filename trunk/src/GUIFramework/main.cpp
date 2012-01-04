#include "guiframework.h"

#include <QtGui>
#include <QApplication>

#include "ConfigWindowManager/configwindowmanager.h"
#include "ConfigWindowManager/UnitTest/configwindowtest1.h"
#include "ConfigWindowManager/UnitTest/configwindowtest2.h"
#include "ConfigWindowManager/UnitTest/configwindowtest3.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    GUIFramework w;
//    w.show();

    ConfigWindowManager cwm;
    ConfigWindowTest1 t1;
    ConfigWindowTest2 t2;
    ConfigWindowTest3 t3;

    cwm.addNewWindow(&t1);
    cwm.addNewWindow(&t2);
    cwm.addNewWindow(&t3);
    cwm.startShow();

    cwm.show();
    return a.exec();
}
