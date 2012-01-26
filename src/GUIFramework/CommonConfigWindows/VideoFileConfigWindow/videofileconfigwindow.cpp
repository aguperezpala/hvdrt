#include "videofileconfigwindow.h"

#include "GUIUtils.h"



/* Read the video file properties */
void VideoFileConfigWindow::readVideoProperties(void)
{
	ASSERT(mImgGenerator);
	cv::VideoCapture *vc = mImgGenerator->getDevice();
	ASSERT(vc);

	double value = vc->get(CV_CAP_PROP_FRAME_COUNT);
	ui.totalFramesLabel->setText(QString::number(value));
	debug("VideoProperties[Total Frames]: %lf\n", value);

	value = vc->get(CV_CAP_PROP_FPS);
	ui.fpsLabel->setText(QString::number(value));
	debug("VideoProperties[FPS]: %lf\n", value);

	value = vc->get(CV_CAP_PROP_POS_FRAMES);
	ui.currentFrameLabel->setText(QString::number(value));
	debug("VideoProperties[CurrentFrame]: %lf\n", value);


}

void VideoFileConfigWindow::updateActualFrameLabel(void)
{
	ASSERT(mImgGenerator);

	cv::VideoCapture *cv = mImgGenerator->getDevice();
	ASSERT(cv);

	// update the current frame
	ui.currentFrameLabel->setText(QString::number(cv->get(CV_CAP_PROP_POS_FRAMES)));
}


/* display frame number x */
void VideoFileConfigWindow::displayFrame(double frame)
{
	ASSERT(mImgGenerator);

	cv::VideoCapture *cv = mImgGenerator->getDevice();
	ASSERT(cv);
	debug("Displaying frame: %lf\n", frame)

	cv->set(CV_CAP_PROP_POS_FRAMES, frame);

	Frame f;
	mImgGenerator->captureFrame(f);
	mDisplayer.setImage(f.data);

	// update the current frame
	updateActualFrameLabel();
}

double VideoFileConfigWindow::getActualFrame(void)
{
	ASSERT(mImgGenerator);

	cv::VideoCapture *cv = mImgGenerator->getDevice();
	ASSERT(cv);

	return cv->get(CV_CAP_PROP_POS_FRAMES);
}





VideoFileConfigWindow::VideoFileConfigWindow(QWidget *parent)
    : ConfigWindow(parent, "VideoFileConfigWindow"),
      mDisplayer(this)
{
	ui.setupUi(this);
	// insert the displayer
	ui.verticalLayout->insertWidget(0,&mDisplayer);

	QObject::connect(ui.prevFrameButton,SIGNAL(clicked(bool)), this,
						SLOT(onPrevClicked(void)));
	QObject::connect(ui.nextFrameButton,SIGNAL(clicked(bool)), this,
						SLOT(onNextClicked(void)));
	QObject::connect(ui.prevFramex2Button,SIGNAL(clicked(bool)), this,
						SLOT(onPrevx2Clicked(void)));
	QObject::connect(ui.nextFramex2Button,SIGNAL(clicked(bool)), this,
						SLOT(onNextx2Clicked(void)));
	QObject::connect(ui.prevFramex3Button,SIGNAL(clicked(bool)), this,
						SLOT(onPrevx3Clicked(void)));
	QObject::connect(ui.nextFramex3Button,SIGNAL(clicked(bool)), this,
						SLOT(onNextx3Clicked(void)));
	QObject::connect(ui.jumpFrameButton,SIGNAL(clicked(bool)), this,
						SLOT(onjumpFrameClicked(void)));

}

VideoFileConfigWindow::~VideoFileConfigWindow()
{

}


/* Set the ImageGenerator to be used */
errCode VideoFileConfigWindow::setImageGenerator(ImageGenerator *ig)
{
	ASSERT(ig);

	// replace the old one if we have one
	mImgGenerator = ig;

	// now check if the ImageGenerator is of type VIDEOFILE
	if(mImgGenerator->getDeviceType() != ImageGenerator::VIDEO_DEV){
		return INVALID_PARAM;
	}

	Frame f;
	if(mImgGenerator->captureFrame(f) != NO_ERROR){
		GUIUtils::showMessageBox("Error getting an image from the video file\n");
		return INTERNAL_ERROR;
	}

	// get the image
	mDisplayer.setImage(f.data);


	readVideoProperties();



	return NO_ERROR;
}

/* Function used to load the configurations from a xml file
 * Returns:
 * 	errCode
 */
errCode VideoFileConfigWindow::loadConfig(const TiXmlElement *)
{
	// do nothing, return no error
	return NO_ERROR;
}

/* Save the data to an xml and return it.
 * Returns:
 * 	0			on Error
 * 	xml			on success */
std::auto_ptr<TiXmlElement> VideoFileConfigWindow::getConfig(void) const
{
	// return 0
	return std::auto_ptr<TiXmlElement> (0);
}

/* Function called when the window get visible */
void VideoFileConfigWindow::windowVisible(void)
{
	if(getActualFrame() < 1.0){
		displayFrame(1.0);
	}
	// update the current frame
	updateActualFrameLabel();
}

/* Function used to retrieve the info about the ConfigWindow. This is
 * an optional function.
 */
QString VideoFileConfigWindow::getInfo(void) const
{
	// TODO: implementar esto!
	return "";
}

/* Function called when the user click on "Next Button" and the ConfigWindow
 * will be hide and show the next one. This function shall return NO_ERROR
 * if the ConfigWindow can be closed, or the error and the string error
 * by param
 */
errCode VideoFileConfigWindow::finish(QString &error)
{
	return NO_ERROR;
}



void VideoFileConfigWindow::onPrevClicked(void)
{
	displayFrame(getActualFrame() - 1.0);
}


void VideoFileConfigWindow::onNextClicked(void)
{
	Frame f;
	mImgGenerator->captureFrame(f);
	mDisplayer.setImage(f.data);

	// update the current frame
	updateActualFrameLabel();
}

void VideoFileConfigWindow::onNextx2Clicked(void)
{
	displayFrame(getActualFrame() + 5.0);
}
void VideoFileConfigWindow::onPrevx2Clicked(void)
{
	displayFrame(getActualFrame() - 5.0);
}
void VideoFileConfigWindow::onNextx3Clicked(void)
{
	displayFrame(getActualFrame() + 15.0);
}
void VideoFileConfigWindow::onPrevx3Clicked(void)
{
	displayFrame(getActualFrame() - 15.0);
}
void VideoFileConfigWindow::onjumpFrameClicked(void)
{
	bool ok;
	double val;
	val = ui.frameText->text().toDouble(&ok);
	if(!ok){
		GUIUtils::showMessageBox("El campo frame debe ser un valor numerico");
		return;
	}
	displayFrame(val);
}



