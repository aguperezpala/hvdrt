#include "videofileconfigwindow.h"

VideoFileConfigWindow::VideoFileConfigWindow(QWidget *parent)
    : ConfigWindow(parent, "VideoFileConfigWindow")
{
	ui.setupUi(this);

}

VideoFileConfigWindow::~VideoFileConfigWindow()
{

}


/* Set the ImageGenerator to be used */
void VideoFileConfigWindow::setImageGenerator(ImageGenerator *ig)
{
	ASSERT(ig);

	// replace the old one if we have one
	mImgGenerator = ig;

	// now check if the ImageGenerator is of type VIDEOFILE
	if(mImgGenerator->getDeviceType() != ImageGenerator::VIDEO_DEV){
		// we have to remove the actual
		mImgGenerator->destroyDevice();
	}

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
