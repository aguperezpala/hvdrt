
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>


#include <qstring.h>
#include <qimage.h>
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
void ImageGeneratorConfigurator::getProperties(void)
{
	ASSERT(mImgGen);
	ASSERT(mImgGen->getDevice());

	cv::VideoCapture *dev = mImgGen->getDevice();
	double value = 0.0;

	// get resolution
	value = dev->get(CV_CAP_PROP_FRAME_WIDTH);
	ui.xres_text->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_FRAME_HEIGHT);
	ui.yres_text->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_FPS);
	ui.framerate_text->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_FOURCC);
	ui.codec_text->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_FORMAT);
	ui.format_text->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_MODE);
	ui.mode_text_2->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_BRIGHTNESS);
	ui.brightness_text_2->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_CONTRAST);
	ui.contrast_text->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_SATURATION);
	ui.format_text_5->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_HUE);
	ui.format_text_6->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_GAIN);
	ui.format_text_8->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_EXPOSURE);
	ui.format_text_9->setText(QString::number(value));

	value = dev->get(CV_CAP_PROP_CONVERT_RGB);
	ui.rgb_text->setText(QString::number(value));


}


////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::showSourceInput(void)
{
	ASSERT(mImgGen);

	if(!mImgGen->getDevice()->isOpened()){
		debug("Error when trying to show the message from a non created device\n");
		GUIUtils::showMessageBox("Error when trying to show the message from "
				"a non created device");
		return;
	}

	mShowInfo = true;

	Frame frame;
	QImage qImg;
	while(mShowInfo){
		// get the frame and show it
		mImgGen->captureFrame(frame);
		GUIUtils::IplImage2QImage(frame.data,qImg);

		// resize the image
		if(frame.data.rows > ui.label_image->minimumHeight() || frame.data.cols > ui.label_image->minimumWidth()){
			// have to set new proportion
			qImg = qImg.scaled(ui.label_image->minimumWidth(), ui.label_image->minimumHeight(), Qt::KeepAspectRatio);
		}

		ui.label_image->setPixmap(QPixmap::fromImage(qImg));

		cv::waitKey(50);
	}
}

////////////////////////////////////////////////////////////////////////////////
ImageGeneratorConfigurator::ImageGeneratorConfigurator(QWidget *parent)
    : GUIConfiguratorDialog(parent, "ImageGeneratorConfigurator"),
      mImgGen(0)
{
	ui.setupUi(this);

	QObject::connect(ui.camera_button,SIGNAL(clicked(bool)), this,
					SLOT(onSourceCameraClicked(void)));
	QObject::connect(ui.file_button,SIGNAL(clicked(bool)), this,
						SLOT(onSourceFileClicked(void)));
	QObject::connect(ui.setParams_button,SIGNAL(clicked(bool)), this,
						SLOT(onSetParamettersClicked(void)));
	QObject::connect(ui.getParams_button,SIGNAL(clicked(bool)), this,
						SLOT(onGetParamettersClicked(void)));
	QObject::connect(ui.done_button,SIGNAL(clicked(bool)), this,
						SLOT(onDoneClicked(void)));
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
	getProperties();

	showSourceInput();
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
	getProperties();
	showSourceInput();


}

////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::onSetParamettersClicked(void)
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

	cv::VideoCapture *dev = mImgGen->getDevice();

	// all the other params
	double value = ui.framerate_text->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_FPS, value);

	value = ui.codec_text->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_FOURCC,value);

	value = ui.format_text->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_FORMAT,value);

	value = ui.mode_text_2->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_MODE,value);

	value = ui.brightness_text_2->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_BRIGHTNESS,value);

	value = ui.contrast_text->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_CONTRAST,value);

	value = ui.format_text_5->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_SATURATION,value);

	value = ui.format_text_6->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_HUE,value);

	value = ui.format_text_8->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_GAIN,value);

	value = ui.format_text_9->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_EXPOSURE,value);

	value = ui.rgb_text->text().toDouble(&ok);
	dev->set(CV_CAP_PROP_CONVERT_RGB,value);


}

////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::onGetParamettersClicked(void)
{
	getProperties();
}

////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::onDoneClicked(void)
{
	// save configuration?
	close();
}


////////////////////////////////////////////////////////////////////////////////
void ImageGeneratorConfigurator::closeEvent(QCloseEvent * event)
{
	debug("CLOSING\n");
	mImgGen->stopGenerating();
	QWidget::closeEvent(event);
}
