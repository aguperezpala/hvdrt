#include "zoneselector.h"

#include <QtGui>
#include <QApplication>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZoneSelector w;
    QString filename = QFileDialog::getOpenFileName(0, "Image", ".", "*");

	if(filename.isEmpty()){
		return -1;
	}

	cv::Mat m = cv::imread(filename.toAscii().data());
	w.setImage(m);
    w.show();

    return a.exec();
}
