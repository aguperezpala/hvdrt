#include <qevent.h>
#include "MouseQLabel.h"


#include <iostream>



/* fills the label with the img */
void MouseQLabel::showImage(const QImage &img)
{
	setPixmap(QPixmap::fromImage(img));
}

/* Remarks the point */
void MouseQLabel::remarkPoint(QImage &img, int x, int y)
{
	int xR = x + REMARK_POINT_PIXELS/2;
	int yB = y + REMARK_POINT_PIXELS/2;

	for(int i = x - REMARK_POINT_PIXELS/2; i < xR; ++i){
		for(int j = y - REMARK_POINT_PIXELS/2; j < yB; ++j){
			img.setPixel(i,j,REMARK_POINT_COLOR);
		}
	}
}

void MouseQLabel::remarkRectangle(QImage &img, int topLeftX, int topLeftY, int bottomRX, int bottomRY)
{
	for(int i = topLeftX; i < bottomRX; ++i){
		img.setPixel(i, topLeftY, REMARK_POINT_COLOR);
	}
	for(int i = topLeftX; i < bottomRX; ++i){
		img.setPixel(i, bottomRY, REMARK_POINT_COLOR);
	}
	for(int i = topLeftY; i < bottomRY; ++i){
		img.setPixel(topLeftX, i, REMARK_POINT_COLOR);
	}
	for(int i = topLeftY; i < bottomRY; ++i){
		img.setPixel(bottomRX, i, REMARK_POINT_COLOR);
	}
}


MouseQLabel::MouseQLabel(QWidget *parent) :
QLabel(parent)
{
	grabMouse();
}

MouseQLabel::~MouseQLabel()
{

}



void MouseQLabel::mousePressEvent(QMouseEvent* event)
{
	mX = event->x();
	mY = event->y();
	std::cout << "MOUSEMOVE: " << mX << " " << mY << std::endl;


	if(mX > mImg.width() || mY > mImg.height()){
		return;
	}

	remarkPoint(mImg, mX, mY);
	showImage(mImg);
}

void MouseQLabel::mouseMoveEvent(QMouseEvent* event)
{
	mLabelX->setText(QString::number(event->x()));
	mLabelY->setText(QString::number(event->y()));
	QImage img = mImg;

	int tlx = (mX < event->x()) ? mX : event->x();
	int tly = (mY < event->y()) ? mY : event->y();
	int brx = (mX > event->x()) ? mX : event->x();
	int bry = (mY > event->y()) ? mY : event->y();

	remarkRectangle(img, tlx,tly,brx,bry);
	showImage(img);

}


/* Sets the image to work with */
void MouseQLabel::setImage(const QImage &img)
{
	mImg = img;
	showImage(mImg);
	adjustSize();
}
