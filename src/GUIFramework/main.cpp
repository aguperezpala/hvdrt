#include "guiframework.h"

#include <QtGui>
#include <QApplication>

#include "DebugUtil.h"

#include "ConfigWindowManager/configwindowmanager.h"
#include "ConfigWindowManager/UnitTest/configwindowtest1.h"
#include "ConfigWindowManager/UnitTest/configwindowtest2.h"
#include "ConfigWindowManager/UnitTest/configwindowtest3.h"
#include "videofileconfigwindow.h"
#include "ImageGenerator.h"
#include "GUIUtils.h"

static void testVideoFileConfigWindow(QApplication &a)
{
	ConfigWindowManager cwm;
	VideoFileConfigWindow vfcw;
	ImageGenerator ig;

	QString filename = QFileDialog::getOpenFileName(0, "Video", ".", "*");

	if(filename.isEmpty()){
		return;
	}

	// create the new one
	if(!ig.createDevice(filename.toAscii().data())){
		GUIUtils::showMessageBox("Error creating the ImageGenerator");
		return;
	}

	ASSERT(vfcw.setImageGenerator(&ig) == NO_ERROR);

	cwm.addNewWindow(&vfcw);
	cwm.startShow();


	cwm.show();


	a.exec();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    GUIFramework w;
//    w.show();

    ConfigWindowManager cwm;
    ConfigWindowTest1 t1;
    ConfigWindowTest2 t2;
    ConfigWindowTest3 t3;
//
//    cwm.addNewWindow(&t1);
//    cwm.addNewWindow(&t2);
//    cwm.addNewWindow(&t3);
//    cwm.startShow();
//
//    cwm.show();


    testVideoFileConfigWindow(a);
    return 0;
}
