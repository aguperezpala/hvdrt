
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>


#include <qstring.h>
#include <qfiledialog.h>

#include "GUIUtils.h"
#include "imagegeneratorconfigurator.h"



////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::showResolution(void)
{
	ASSERT(mImgGen);
	cv::VideoCapture *dev = mImgGen->getDevice();
	ASSERT(dev);

	double xres = dev->get(CV_CAP_PROP_FRAME_WIDTH);
	double yres = dev->get(CV_CAP_PROP_FRAME_HEIGHT);

	ui.xres_text->setText(QString::number(xres));
	ui.yres_text->setText(QString::number(yres));
}

////////////////////////////////////////////////////////////////////////////////
bool ImageGeneratorConfigurator::setResolution(double width, double height)
{
	ASSERT(mImgGen);
	ASSERT(mImgGen->getDevice());

	// change the resolution
	mImgGen->getDevice()->set(CV_CAP_PROP_FRAME_WIDTH, width);
	mImgGen->getDevice()->set(CV_CAP_PROP_FRAME_HEIGHT, height);


	if(mImgGen->getDevice()->set(CV_CAP_PROP_FRAME_WIDTH, width) == false){
		GUIUtils::showMessageBox("Couldn't set the Width (x) resolution");
		return false;
	}
	if(mImgGen->getDevice()->set(CV_CAP_PROP_FRAME_HEIGHT, height) == false){
		GUIUtils::showMessageBox("Couldn't set the Height (y) resolution");
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
ImageGeneratorConfigurator::ImageGeneratorConfigurator(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.camera_button,SIGNAL(clicked(bool)), this,
					SLOT(onSourceCameraClicked(void)));
	QObject::connect(ui.file_button,SIGNAL(clicked(bool)), this,
						SLOT(onSourceFileClicked(void)));
	QObject::connect(ui.resolution_button,SIGNAL(clicked(bool)), this,
							SLOT(onSetResolutionClicked(void)));


}

////////////////////////////////////////////////////////////////////////////////
ImageGeneratorConfigurator::~ImageGeneratorConfigurator()
{

}

////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::setImgGenerator(ImageGenerator *imgGen)
{
	ASSERT(imgGen);
	mImgGen = imgGen;
	// add the tester
	mFrameProcTester.reset(new FrameProcessorTester());
	mImgGen->addNewListener(mFrameProcTester.get());

}

////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::onSourceCameraClicked(void)
{
	ASSERT(mImgGen);
	// close the actual image generator
	mImgGen->stopGenerating();
	mImgGen->destroyDevice();

	// create the new one
	if(!mImgGen->createDevice()){
		GUIUtils::showMessageBox("Error creating the ImageGenerator");
		return;
	}

	// if there was no error, then we start
	showResolution();
	mImgGen->startGenerating();
}

////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::onSourceFileClicked(void)
{
	ASSERT(mImgGen);

	// open a file
	QString filename = QFileDialog::getOpenFileName(this, "Image", ".", "*");

	if(filename.isEmpty()){
		return;
	}

	// close the actual image generator
	mImgGen->stopGenerating();
	mImgGen->destroyDevice();

	// create the new one
	if(!mImgGen->createDevice(filename.toAscii().data())){
		GUIUtils::showMessageBox("Error creating the ImageGenerator");
		return;
	}

	// if there was no error, then we start
	showResolution();
	mImgGen->startGenerating();

}

////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::onSetResolutionClicked(void)
{
	ASSERT(mImgGen);
	ASSERT(mImgGen->getDevice());

	// get the old resolution
	double xres = mImgGen->getDevice()->get(CV_CAP_PROP_FRAME_WIDTH);
	double yres = mImgGen->getDevice()->get(CV_CAP_PROP_FRAME_HEIGHT);

	// change the resolution
	bool ok = false;
	double x = ui.xres_text->text().toDouble(&ok);
	if(!ok){
		GUIUtils::showMessageBox("You must set a number to the x resolution");
	}
	double y = ui.yres_text->text().toDouble(&ok);
	if(!ok){
		GUIUtils::showMessageBox("You must set a number to the y resolution");
	}
	if(!setResolution(x,y)){
		// set the old resolution
		setResolution(xres, yres);
	}

}

////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::closeEvent(QCloseEvent * event)
{
	debug("CLOSING\n");
	mImgGen->removeFrameListener(mFrameProcTester.get());
	QWidget::closeEvent(event);
}
