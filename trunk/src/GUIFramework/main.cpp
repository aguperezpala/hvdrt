#include "guiframework.h"

#include <QtGui>
#include <QApplication>

#include "DebugUtil.h"

#include "ConfigWindowManager/configwindowmanager.h"
#include "ConfigWindowManager/UnitTest/configwindowtest1.h"
#include "ConfigWindowManager/UnitTest/configwindowtest2.h"
#include "ConfigWindowManager/UnitTest/configwindowtest3.h"

#include "PerspectiveRectifier.h"

#include "videofileconfigwindow.h"
#include "guiperspectiverectifier.h"
#include "guicannyborderdetector.h"
#include "CannyBorderDetector.h"
#include "WaveHeightAnalyzer.h"

#include "guimiddlepointclipping.h"
#include "MiddlePointClipping.h"

#include "ImageGenerator.h"
#include "GUIUtils.h"
#include "XmlHelper.h"


// load/save config tester
static bool testLoadSaveConfig(ConfigWindow &cw)
{
	std::auto_ptr<TiXmlElement> l = cw.getConfig();

	if(!l.get()){
		debug("ConfigWindow %s returns an empty config\n", cw.getName().c_str());
		return false;
	}

	errCode err = cw.loadConfig(l.get());
	if(err != NO_ERROR){
		debug("ConfigWindow %s error when loading config: %d\n",
				cw.getName().c_str(), err);
		return false;
	}

	// check loading some invalid xml
	std::auto_ptr<TiXmlElement> emp(0);
	err = cw.loadConfig(emp.get());
	if(err == NO_ERROR){
		debug("ConfigWindow %s accepts an invalid xml!!!\n", cw.getName().c_str());
		return false;
	}

	return true;
}

static void testVideoFileConfigWindow(QApplication &a)
{
	ConfigWindowManager cwm(0,800,600);
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

static void testGuiPerspectiveRectifier(QApplication &a)
{
	ConfigWindowManager cwm(0,a.desktop()->width(),a.desktop()->height());
	cwm.showMaximized();
	cwm.activateWindow();
	cwm.raise();
	ImageGenerator ig;
	PerspectiveRectifier pr;

	QString filename = QFileDialog::getOpenFileName(0, "Video", ".", "*");

	if(filename.isEmpty()){
		return;
	}

	// create the new one
	if(!ig.createDevice(filename.toAscii().data())){
		GUIUtils::showMessageBox("Error creating the ImageGenerator");
		return;
	}

	GUIPerspectiveRectifier gpr(&ig);
	gpr.setPerspectiveRectifierIP(&pr);
	gpr.updateImg();


	cwm.addNewWindow(&gpr);
	cwm.startShow();

	cwm.show();


	a.exec();
}

static void testGuiBorderDetector(QApplication &a)
{
	ConfigWindowManager cwm(0,a.desktop()->width(),a.desktop()->height());
	cwm.showMaximized();
	cwm.activateWindow();
	cwm.raise();

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
	GUICannyBorderDetector gcbd(&ig);
	CannyBorderDetector cbd;
	WaveHeightAnalyzer wha;

	gcbd.setCannyImgProcessor(&cbd);
	PerspectiveRectifier pr;

	GUIPerspectiveRectifier gpr(&ig);
	gpr.setPerspectiveRectifierIP(&pr);
	gpr.setWaveHeightAnalyzerIP(&wha);

	gcbd.setPerspectiveRectifierIP(&pr);

	cwm.addNewWindow(&gpr);
	cwm.addNewWindow(&gcbd);
	cwm.startShow();
	cwm.show();

	a.exec();
}

static void testGuiMiddlePointClipping(QApplication &a)
{
	ConfigWindowManager cwm(0,a.desktop()->width(),a.desktop()->height());
	cwm.showMaximized();
	cwm.activateWindow();
	cwm.raise();

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

	CannyBorderDetector cbd;
	WaveHeightAnalyzer wha;
	MiddlePointClipping mpc;
	GUIMiddlePointClipping gmpc(&ig);
	PerspectiveRectifier pr;

	gmpc.setMiddlePointClippingIP(&mpc);
	gmpc.setPerspectiveRectifierIP(&pr);
	gmpc.setWaveHeightAnalyzerIP(&wha);

	GUIPerspectiveRectifier gpr(&ig);
	gpr.setPerspectiveRectifierIP(&pr);
	gpr.setWaveHeightAnalyzerIP(&wha);

	cwm.addNewWindow(&gpr);
	cwm.addNewWindow(&gmpc);
	cwm.addNewWindow(&gmpc);

	// load the autosave config
	std::auto_ptr<TiXmlDocument> conf(XmlHelper::loadFromFile("autosave.xml"));
	ASSERT(conf.get());
	if(cwm.loadConfig(conf->RootElement()) != NO_ERROR){
		GUIUtils::showMessageBox("Error cargando el autosave.xml");
	}

	cwm.startShow();
	cwm.show();

	a.exec();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    GUIFramework w;
//    w.show();
//
//    ConfigWindowManager cwm;
//    ConfigWindowTest1 t1;
//    ConfigWindowTest2 t2;
//    ConfigWindowTest3 t3;
//
//    cwm.addNewWindow(&t1);
//    cwm.addNewWindow(&t2);
//    cwm.addNewWindow(&t3);
//    cwm.startShow();
//
//    cwm.show();


//    testVideoFileConfigWindow(a);
//    testGuiPerspectiveRectifier(a);
//    testGuiBorderDetector(a);
    testGuiMiddlePointClipping(a);


    return 0;
}
