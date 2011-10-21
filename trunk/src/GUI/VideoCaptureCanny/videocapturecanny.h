#ifndef VIDEOCAPTURECANNY_H
#define VIDEOCAPTURECANNY_H

#include <QtGui/QWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>

#include "ui_videocapturecanny.h"

class VideoCaptureCanny : public QWidget
{
    Q_OBJECT

public:
    VideoCaptureCanny(QWidget *parent = 0);
    ~VideoCaptureCanny();

public slots:
	void onVideoLoadClick(void);
	void onCameraClick(void);
	void onButtonCannyPressed(void);


private:
	int showMessage(const QString &m);

	bool startCapturing(void);

private:
    Ui::VideoCaptureCannyClass ui;

    // thresholds
    double					mCannyTh1;
    double					mCannyTh2;

    // video capture
    cv::VideoCapture		mVideoCapture;
};

#endif // VIDEOCAPTURECANNY_H
