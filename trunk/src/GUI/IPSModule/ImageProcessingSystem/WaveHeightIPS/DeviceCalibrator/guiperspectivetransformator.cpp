#include <qmessagebox.h>
#include <qfiledialog.h>

#include <iostream>
#include <cassert>

#include "guiperspectivetransformator.h"


///////////////////////////////////////////////////////////////////////////////
int GUIPerspectiveTransformator::showMessage(const QString &m)
{
	QMessageBox msg(this);
	msg.setText(m);
	msg.exec();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
bool GUIPerspectiveTransformator::IplImage2QImage(const cv::Mat &iplImgMat, QImage &ref)
{
	IplImage iplImg = iplImgMat;
	int h = iplImg.height;
	int w = iplImg.width;
	int channels = iplImg.nChannels;
	QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
	char *data = iplImg.imageData;

	for (int y = 0; y < h; y++, data += iplImg.widthStep)
	{
		for (int x = 0; x < w; x++)
		{
			char r, g, b, a = 0;
			if (channels == 1)
			{
				r = data[x * channels];
				g = data[x * channels];
				b = data[x * channels];
			}
			else if (channels == 3 || channels == 4)
			{
				r = data[x * channels + 2];
				g = data[x * channels + 1];
				b = data[x * channels];
			}

			if (channels == 4)
			{
				a = data[x * channels + 3];
				qimg->setPixel(x, y, qRgba(r, g, b, a));
			}
			else
			{
				qimg->setPixel(x, y, qRgb(r, g, b));
			}
		}
	}
	ref = *qimg;
	delete qimg;


	return true;
}


///////////////////////////////////////////////////////////////////////////////
GUIPerspectiveTransformator::GUIPerspectiveTransformator(QWidget *parent)
    : QDialog(parent),
      mPointsVector(0),
      mTransformator(0)
{
	ui.setupUi(this);
	mMouseLabel = new MouseQLabel(this);
	mMouseLabel->setObjectName(QString::fromUtf8("img_label"));
	mScrollArea.setWidget(mMouseLabel);
	ui.verticalLayout->insertWidget(0,&mScrollArea);
	mMouseLabel->setLabelX(ui.x_label);
	mMouseLabel->setLabelY(ui.y_label);
	mMouseLabel->setZoomLabel(ui.zoom_label);

	mMouseLabel->grabMouse();
	mMouseLabel->setMouseTracking(true);

	QObject::connect(ui.clear_button,SIGNAL(clicked(bool)), this,
				SLOT(onClearClicked(void)));
	QObject::connect(ui.done_button,SIGNAL(clicked(bool)), this,
					SLOT(onDoneClicked(void)));
	QObject::connect(ui.preview_button,SIGNAL(clicked(bool)), this,
						SLOT(onPreviewClicked(void)));
}

///////////////////////////////////////////////////////////////////////////////
GUIPerspectiveTransformator::~GUIPerspectiveTransformator()
{
	if(mMouseLabel){
		delete mMouseLabel;
	}
}



///////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveTransformator::setImage(const cv::Mat &img)
{
	if(!mImage.empty()){
		mImage.release();
	}

	mImage = img.clone();
	QImage qimg;
	IplImage2QImage(mImage, qimg);
	mMouseLabel->setImage(qimg);
}


///////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveTransformator::setPointsVector(std::vector<cv::Point2i> *vec)
{
	assert(mPointsVector == 0);
	mPointsVector = vec;
}

///////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveTransformator::onClearClicked(void)
{
	mMouseLabel->clearPoints();
}

///////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveTransformator::onDoneClicked(void)
{
	showMessage("We have " + QString::number(mMouseLabel->getPoints().size()) + " points");
	assert(mPointsVector);

	mPointsVector->clear();
	const std::vector<QPoint> &points = mMouseLabel->getPoints();
	for(int i = 0; i < points.size(); ++i){
		mPointsVector->push_back(cv::Point(points[i].x(), points[i].y()));
	}

	close();
}

///////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveTransformator::onPreviewClicked(void)
{
	const std::vector<QPoint> &points = mMouseLabel->getPoints();

	if(points.size() != 4) {
		showMessage("We required 4 corners points in the next order:"
				"TopLeft, TopRight, BottomLeft, BottomRight");

		return;
	}

	// we have the points, set it
	cv::Point2f	c1[4];
	cv::Point2f	c2[4];

	for(int i = 0; i < points.size(); ++i){
		c1[i].x = points[i].x();	c1[i].y = points[i].y();
	}

	c2[0].x = 0;			c2[0].y = 0;
	c2[1].x = mImage.cols;	c2[1].y = 0;
	c2[2].x = 0;			c2[2].y = mImage.rows;
	c2[3].x = mImage.cols;	c2[3].y = mImage.rows;

	cv::Mat tm = cv::getPerspectiveTransform(c1,c2);
	std::cout << "cols: " << mImage.cols << " rows: " << mImage.rows << std::endl;
	cv::Mat m = mImage.clone();
	cv::warpPerspective(mImage,m,tm, mImage.size());

	cv::namedWindow("TransformedImg", 0);
	cv::imshow("TransformedImg", m);

}


PerspectiveTransformator *GUIPerspectiveTransformator::getPerspectiveTransformator(void)
{
	const std::vector<QPoint> &points = mMouseLabel->getPoints();
	for(int i = 0; i < points.size(); ++i){
		mPointsVector->push_back(cv::Point(points[i].x(), points[i].y()));
	}

	if(points.size() != 4){
		showMessage("Error: We have to choose 4 points to create the perspective transformation\n");
		return 0;
	}

	// create the mTransformator
	if(mTransformator){
		showMessage("Warning: There was already created a transformator before");
		mTransformator = 0;
	}
	// create the new one
	mTransformator = new PerspectiveTransformator();
	ASSERT(mTransformator);

	// by now we will create the result image of the size of the actual image
	std::vector<cv::Point2f> sourcePoints;
	std::vector<cv::Point2f> destPoints;

	// load the source points
	for(int i = 0; i < points.size(); ++i){
		sourcePoints.push_back(cv::Point2f(points[i].x(),points[i].y()));
	}
	// add the dest points
	cv::Point2f	c2[4];
	c2[0].x = 0;			c2[0].y = 0;
	c2[1].x = mImage.cols;	c2[1].y = 0;
	c2[2].x = 0;			c2[2].y = mImage.rows;
	c2[3].x = mImage.cols;	c2[3].y = mImage.rows;
	for(int i = 0; i < 4; ++i){
		destPoints.push_back(c2[i]);
	}

	if(!mTransformator->setTransformationPoints(sourcePoints, destPoints)){
		showMessage("Error creating the PerspectiveTransformator\n");
		delete mTransformator;
		mTransformator = 0;
	}

	return mTransformator;
}


