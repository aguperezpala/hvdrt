#include <qevent.h>
#include "MouseQLabel.h"


#include <iostream>


MouseQLabel::MouseQLabel(QWidget *parent) :
QLabel(parent)
{

}

MouseQLabel::~MouseQLabel()
{

}



void MouseQLabel::mousePressEvent(QMouseEvent* event)
{
	mX = event->x();
	mY = event->y();
	std::cout << "MOUSEMOVE: " << mX << " " << mY << std::endl;

	mLabelX->setText(QString(mX));
	mLabelY->setText(QString(mY));
}
