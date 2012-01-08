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
	ui.frameSlider->setMaximum(value-1);

	value = vc->get(CV_CAP_PROP_FPS);
	ui.fpsLabel->setText(QString::number(value));

	value = vc->get(CV_CAP_PROP_POS_FRAMES);
	ui.currentFrameLabel->setText(QString::number(value));
	ui.frameSlider->setValue(value);

}





VideoFileConfigWindow::VideoFileConfigWindow(QWidget *parent)
    : ConfigWindow(parent, "VideoFileConfigWindow"),
      mDisplayer(this)
{
	ui.setupUi(this);
	// insert the displayer
	ui.verticalLayout->insertWidget(0,&mDisplayer);

	QObject::connect(ui.frameSlider,SIGNAL(valueChanged(int)), this,
						SLOT(onSlideChange(int)));

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
	// TODO: implementar esto!
}

/* Save the data to an xml and return it.
 * Returns:
 * 	0			on Error
 * 	xml			on success */
std::auto_ptr<TiXmlElement> VideoFileConfigWindow::getConfig(void) const
{
	// TODO: implementar esto!
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
	// TODO: implementar esto!
}



void VideoFileConfigWindow::onSlideChange(int value)
{
	ASSERT(mImgGenerator);

	cv::VideoCapture *cv = mImgGenerator->getDevice();
	ASSERT(cv);

	// update the video to the current frame
	cv->set(CV_CAP_PROP_POS_FRAMES, value);

	// get the new frame and show it
	Frame f;
	mImgGenerator->captureFrame(f);
	mDisplayer.setImage(f.data);

	// update the current frame
	ui.currentFrameLabel->setText(QString::number(cv->get(CV_CAP_PROP_POS_FRAMES)));

}





