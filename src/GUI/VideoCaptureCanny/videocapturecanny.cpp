#include <qmessagebox.h>
#include <qfiledialog.h>

#include "videocapturecanny.h"

///////////////////////////////////////////////////////////////////////////////
int VideoCaptureCanny::showMessage(const QString &m)
{
	QMessageBox msg(this);
	msg.setText(m);
	msg.exec();

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
void VideoCaptureCanny::onVideoLoadClick(void)
{
	QString filename = QFileDialog::getOpenFileName(this, "Image", ".", "*");

	if(filename.isEmpty()){
		return;
	}

	if(mVideoCapture.isOpened()){
		mVideoCapture.release();
	}

	mVideoCapture.open(filename.toAscii().data());
	if(!mVideoCapture.isOpened()){
		showMessage("Error opening the file " + filename);
		return;
	}

	startCapturing();
}

///////////////////////////////////////////////////////////////////////////////
void VideoCaptureCanny::onCameraClick(void)
{
	if(mVideoCapture.isOpened()){
		mVideoCapture.release();
	}

	mVideoCapture.open(0);
	if(!mVideoCapture.isOpened()){
		showMessage("Error capturing from Camera");
		return;
	}

	startCapturing();
}

///////////////////////////////////////////////////////////////////////////////
void VideoCaptureCanny::onButtonCannyPressed(void)
{
	bool ok;

	mCannyTh1 = ui.cannyA_text->text().toDouble(&ok);
	if(!ok){
		showMessage("Canny parameters must be numbers");
		return;
	}

	mCannyTh2 = ui.cannyB_text->text().toDouble(&ok);
	if(!ok){
			showMessage("Canny parameters must be numbers");
			return;
		}
}

///////////////////////////////////////////////////////////////////////////////
bool VideoCaptureCanny::startCapturing(void)
{
	cv::Mat edges;
	cv::namedWindow("edges",1);
	for(;;)
	{
		cv::Mat frame;

		mVideoCapture >> frame; // get a new frame from camera
		cv::cvtColor(frame, edges, CV_BGR2GRAY);
		cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
		cv::Canny(edges, edges, mCannyTh1, mCannyTh2, 3);
		cv::imshow("edges", edges);

		if(cv::waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return true;

}


VideoCaptureCanny::VideoCaptureCanny(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	// connect the signals
	QObject::connect(ui.camera_button,SIGNAL(clicked(bool)), this,
				SLOT(onCameraClick(void)));
	QObject::connect(ui.canny_button,SIGNAL(clicked(bool)), this,
					SLOT(onButtonCannyPressed(void)));
	QObject::connect(ui.loadVideo_button,SIGNAL(clicked(bool)), this,
					SLOT(onVideoLoadClick(void)));

	ui.cannyA_text->setText("10");
	ui.cannyB_text->setText("80");

	mCannyTh1 = ui.cannyA_text->text().toDouble();
	mCannyTh2 = ui.cannyB_text->text().toDouble();
}

VideoCaptureCanny::~VideoCaptureCanny()
{

}
