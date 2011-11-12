#include <qevent.h>
#include "MouseQLabel.h"
#include "DebugUtil.h"

#include <iostream>



/* fills the label with the img */
void MouseQLabel::showImage(const QImage &img)
{
	setPixmap(QPixmap::fromImage(img));
	setFixedHeight(img.height());
	setFixedWidth(img.width());
}

/* Remarks the point */
void MouseQLabel::remarkPoint(QImage &img, int x, int y)
{
	int xR = x + REMARK_POINT_PIXELS/2;
	int yB = y + REMARK_POINT_PIXELS/2;


	for(int i = x - REMARK_POINT_PIXELS/2; i < xR; ++i){
		for(int j = y - REMARK_POINT_PIXELS/2; j < yB; ++j){
			img.setPixel(i,j,mPaintColor);
		}
	}
}

void MouseQLabel::remarkListPoints(int num, QImage &img)
{
	num = (num < mPoints.size()) ? num : mPoints.size();

	for(int i = 0; i < num; ++i){
		remarkPoint(img, mPoints[i].x(), mPoints[i].y());
	}
}

void MouseQLabel::remarkRectangle(QImage &img, int topLeftX, int topLeftY, int bottomRX, int bottomRY)
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


MouseQLabel::MouseQLabel(QWidget *parent) :
QLabel(parent)
{
	mPaintColor = qRgb(255,0,0);
}

MouseQLabel::~MouseQLabel()
{

}



void MouseQLabel::mousePressEvent(QMouseEvent* event)
{
	mX = event->x();
	mY = event->y();


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

void MouseQLabel::mouseMoveEvent(QMouseEvent* event)
{
	mLabelX->setText(QString::number(event->x()));
	mLabelY->setText(QString::number(event->y()));

	// create the zoomed image
	int factor = ZOOM_IMAGE_SIZE / 2;
	QImage zoomed = mImg.copy(event->x() - factor, event->y() - factor,
			ZOOM_IMAGE_SIZE,
			ZOOM_IMAGE_SIZE);
	// paint the middle point to see where we will select the point
	zoomed.setPixel(factor, factor, mPaintColor);
	mZoomLabel->setPixmap(QPixmap::fromImage(zoomed.scaled(QSize(ZOOM_FACTOR,
			ZOOM_FACTOR))));
//	mZoomLabel->setPixmap(QPixmap::fromImage(zoomed));

}



/* Sets the image to work with */
void MouseQLabel::setImage(const QImage &img)
{
	mOrigImg = img;
	mImg = img;
	showImage(mImg);
	adjustSize();
}

void MouseQLabel::setZoomLabel(QLabel *zoom)
{
	ASSERT(zoom);
	mZoomLabel = zoom;
}

/* clears the points */
void MouseQLabel::clearPoints(void)
{
	mImg = mOrigImg;
	mPoints.clear();
	showImage(mImg);
}

