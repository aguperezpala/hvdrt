#include "cameraconfigwindow.h"
#include <cstdlib>


CameraConfigWindow::CameraConfigWindow(QWidget *parent)
    : ConfigWindow(parent, "CameraConfigWindow"),
      mImgGenerator(0),
      mFrameDisplayer(this),
      mTimerId(-1)
{
	ui.setupUi(this);

	// add the frame displayer
	ui.scrollArea->setWidget(&mFrameDisplayer);

	QObject::connect(ui.qv4lButton,SIGNAL(clicked(bool)), this,
				SLOT(onqv4lClicked(void)));
}

CameraConfigWindow::~CameraConfigWindow()
{

}

////////////////////////////////////////////////////////////////////////////////
errCode CameraConfigWindow::setImageGenerator(ImageGenerator *ig)
{
	ASSERT(ig);

	// replace the old one if we have one
	mImgGenerator = ig;

	// now check if the ImageGenerator is of type VIDEOFILE
	if(mImgGenerator->getDeviceType() != ImageGenerator::CAMERA_DEV){
		// we have to remove the actual
		return INVALID_PARAM;
	}

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode CameraConfigWindow::loadConfig(const TiXmlElement *)
{
	// TODO: implementar esto!

	return FEATURE_NOT_SUPPORTED;
}

////////////////////////////////////////////////////////////////////////////////
std::auto_ptr<TiXmlElement> CameraConfigWindow::getConfig(void) const
{
	// TODO:
	return std::auto_ptr<TiXmlElement> (0);
}

////////////////////////////////////////////////////////////////////////////////
QString CameraConfigWindow::getInfo(void) const
{
	return "Configure la camara.";
}

////////////////////////////////////////////////////////////////////////////////
void  CameraConfigWindow::windowVisible(void)
{
	// start timer
	if(mTimerId > 0){
		killTimer(mTimerId);
	}
	mTimerId = startTimer(FRAME_TIME_UPDATE);
}

////////////////////////////////////////////////////////////////////////////////
void  CameraConfigWindow::windowInvisible(void)
{
	if(mTimerId > 0){
		killTimer(mTimerId);
		mTimerId = -1;
	}
}


////////////////////////////////////////////////////////////////////////////////
errCode CameraConfigWindow::finish(QString &error)
{
	// TODO: implementar esto!

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
void CameraConfigWindow::onqv4lClicked(void)
{
	// destroy the device and then create it agan, stop the timer
	killTimer(mTimerId);
	mImgGenerator->destroyDevice();
	std::system(QV4L_COMMAND_PATH);
	mImgGenerator->createDevice(0);
	mTimerId = startTimer(FRAME_TIME_UPDATE);
}

////////////////////////////////////////////////////////////////////////////////
void CameraConfigWindow::timerEvent(QTimerEvent *e)
{
	if(e->timerId() == mTimerId){
		// update the frame
		mImgGenerator->captureFrame(mFrame);
		mFrameDisplayer.setImage(mFrame.data);
	}

}
