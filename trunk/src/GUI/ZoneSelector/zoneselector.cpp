#include <qmessagebox.h>
#include <qfiledialog.h>

#include <iostream>
#include <cassert>

#include "zoneselector.h"


///////////////////////////////////////////////////////////////////////////////
int ZoneSelector::showMessage(const QString &m)
{
	QMessageBox msg(this);
	msg.setText(m);
	msg.exec();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
bool ZoneSelector::IplImage2QImage(const cv::Mat &iplImgMat, QImage &ref)
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
ZoneSelector::ZoneSelector(QWidget *parent)
    : QWidget(parent),
      mPointsVector(0)
{
	ui.setupUi(this);
	mMouseLabel = new MouseQLabel(this);
	mMouseLabel->setObjectName(QString::fromUtf8("img_label"));
	ui.verticalLayout->insertWidget(0,mMouseLabel);
	mMouseLabel->setLabelX(ui.x_label);
	mMouseLabel->setLabelY(ui.y_label);

	QString filename = QFileDialog::getOpenFileName(this, "Image", ".", "*");

	if(filename.isEmpty()){
		return;
	}


	mImage = cv::imread(filename.toAscii().data());
	QImage img;
	IplImage2QImage(mImage, img);
	mMouseLabel->setImage(img);
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
ZoneSelector::~ZoneSelector()
{
	if(mMouseLabel){
		delete mMouseLabel;
	}
}



///////////////////////////////////////////////////////////////////////////////
void ZoneSelector::setImage(const cv::Mat &img)
{
	if(!mImage.empty()){
		mImage.release();
	}

	mImage = img.clone();
}


///////////////////////////////////////////////////////////////////////////////
void ZoneSelector::setPointsVector(std::vector<cv::Point2i> *vec)
{
	assert(mPointsVector == 0);
	mPointsVector = vec;
}

///////////////////////////////////////////////////////////////////////////////
void ZoneSelector::onClearClicked(void)
{
	mMouseLabel->clearPoints();
}

///////////////////////////////////////////////////////////////////////////////
void ZoneSelector::onDoneClicked(void)
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
void ZoneSelector::onPreviewClicked(void)
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


