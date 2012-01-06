
#include <qevent.h>
#include "zoomedlabel.h"

#include "DebugUtil.h"

ZoomedLabel::ZoomedLabel(QWidget *parent)
    : QLabel(parent),
      mLabelX(0),
      mLabelY(0),
      mZoomLabel(0)
{
	mPaintColor = qRgb(255,0,0);
	grabMouse();
	setMouseTracking(true);
}

ZoomedLabel::~ZoomedLabel()
{

}




/* fills the label with the img */
void ZoomedLabel::showImage(const QImage &img)
{
	setPixmap(QPixmap::fromImage(img));
	setFixedHeight(img.height());
	setFixedWidth(img.width());
}

/* Remarks the point */
void ZoomedLabel::remarkPoint(QImage &img, int x, int y)
{
	int xR = x + REMARK_POINT_PIXELS/2;
	int yB = y + REMARK_POINT_PIXELS/2;


	for(int i = x - REMARK_POINT_PIXELS/2; i < xR; ++i){
		for(int j = y - REMARK_POINT_PIXELS/2; j < yB; ++j){
			img.setPixel(i,j,mPaintColor);
		}
	}
}

void ZoomedLabel::remarkListPoints(int num, QImage &img)
{
	num = (num < mPoints.size()) ? num : mPoints.size();

	for(int i = 0; i < num; ++i){
		remarkPoint(img, mPoints[i].x(), mPoints[i].y());
	}
}

void ZoomedLabel::remarkRectangle(QImage &img, int topLeftX, int topLeftY, int bottomRX, int bottomRY)
{
	for(int i = topLeftX; i < bottomRX; ++i){
		img.setPixel(i, topLeftY, mPaintColor);
	}
	for(int i = topLeftX; i < bottomRX; ++i){
		img.setPixel(i, bottomRY, mPaintColor);
	}
	for(int i = topLeftY; i < bottomRY; ++i){
		img.setPixel(topLeftX, i, mPaintColor);
	}
	for(int i = topLeftY; i < bottomRY; ++i){
		img.setPixel(bottomRX, i, mPaintColor);
	}
}



/* Add a new point */
void ZoomedLabel::addPoint(int x, int y)
{
	mX = x;
	mY = y;


	if(mX > mImg.width() || mY > mImg.height()){
		return;
	}

	// add the new point
	QPoint p(mX, mY);
	mPoints.push_back(p);

	// show the new point
	remarkPoint(mImg, mX, mY);
	showImage(mImg);
}

void ZoomedLabel::mousePressEvent(QMouseEvent* event)
{
	mX = event->x();
	mY = event->y();


	if(mX > mImg.width() || mY > mImg.height()){
		return;
	}

	addPoint(mX, mY);

}

void ZoomedLabel::mouseMoveEvent(QMouseEvent* event)
{
	if(mLabelX){
		mLabelX->setText(QString::number(event->x()));
	}
	if(mLabelY){
		mLabelY->setText(QString::number(event->y()));
	}

	// create the zoomed image
	int factor = ZOOM_IMAGE_SIZE * 0.5;
	QImage zoomed = mImg.copy(event->x() - factor, event->y() - factor,
			ZOOM_IMAGE_SIZE,
			ZOOM_IMAGE_SIZE);
	// paint the middle point to see where we will select the point
	zoomed.setPixel(factor, factor, mPaintColor);
	ASSERT(mZoomLabel);
	mZoomLabel->setPixmap(QPixmap::fromImage(zoomed.scaled(QSize(ZOOM_FACTOR,
			ZOOM_FACTOR))));
	mZoomLabel->setMaximumHeight(ZOOM_IMAGE_SIZE);
	mZoomLabel->setMaximumWidth(ZOOM_IMAGE_SIZE);

//	mZoomLabel->setPixmap(QPixmap::fromImage(zoomed));

}



/* Sets the image to work with */
void ZoomedLabel::setImage(const QImage &img)
{
	mOrigImg = img;
	mImg = img;
	showImage(mImg);
	adjustSize();
}

void ZoomedLabel::setZoomLabel(QLabel *zoom)
{
	ASSERT(zoom);
	mZoomLabel = zoom;
}


/* clears the points */
void ZoomedLabel::clearPoints(void)
{
	mImg = mOrigImg;
	mPoints.clear();
	showImage(mImg);
}
