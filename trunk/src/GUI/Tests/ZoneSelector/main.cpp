#include "zoneselector.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZoneSelector w;
    w.show();
    return a.exec();
}
