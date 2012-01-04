#include "framelabeldisplayer.h"

#include "GUIUtils.h"
#include "DebugUtil.h"

FrameLabelDisplayer::FrameLabelDisplayer(QWidget *parent)
    : QLabel(parent)
{
	ui.setupUi(this);
}

FrameLabelDisplayer::~FrameLabelDisplayer()
{

}


/* Set the image to be shown */
void FrameLabelDisplayer::setImage(const cv::Mat &img)
{
	GUIUtils::IplImage2QImage(img,mShowImage);
	if(mShowImage.isNull()){
		debug("Error while converting the image\n");
		return;
	}

	// set the content
	setScaledContents(true);
	setPixmap(QPixmap::fromImage(mShowImage));

}
