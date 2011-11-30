
#include <QtGui>
#include <QApplication>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ImageGenerator.h"
#include "guiwaveheightips.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRect screen = a.desktop()->screenGeometry();

    GUIWaveHeightIPS whips(0, screen.width(), screen.height());

    whips.show();
    int err = whips.initialize();
    if(err != NO_ERROR){
    	debug("Error initializing whips: %d\n", err);
    	return -1;
    }

    err = whips.execute();
    if(err != NO_ERROR){
    	debug("Error executing whips: %d\n", err);
	}


    return a.exec();
}
