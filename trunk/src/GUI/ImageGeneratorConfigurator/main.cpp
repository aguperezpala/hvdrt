#include "imagegeneratorconfigurator.h"

#include <QtGui>
#include <QApplication>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ImageGenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageGenerator ig;

    ImageGeneratorConfigurator w;
    w.setImgGenerator(&ig);
    w.show();

    return a.exec();
}
