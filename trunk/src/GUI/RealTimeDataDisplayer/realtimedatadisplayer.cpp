#include <qpainter.h>
#include <qgraphicsitem.h>
#include <qlist.h>
#include <qtextdocument.h>

#include <iostream>
#include <cassert>

#include "realtimedatadisplayer.h"



///////////////////////////////////////////////////////////////////////////////
void RealTimeDataDisplayer::createAxis(void)
{
	assert(mXMaxValue > 0);
	assert(mYMaxValue > 0);
	assert(mMeasureScale > 0);
	assert(mTimeScale > 0);

	float xLen = mXMaxValue * mTimeScale;
	float yLen = mYMaxValue * mMeasureScale;

	// create the X axis
	QGraphicsLineItem *xAxis = new QGraphicsLineItem(0,0.0,xLen,0.0);
	mScene.addItem(xAxis);


	// creates the Y axis
	QGraphicsLineItem *yAxis = new QGraphicsLineItem(0.0,0,0.0,-yLen);
	mScene.addItem(yAxis);


}

///////////////////////////////////////////////////////////////////////////////
void RealTimeDataDisplayer::createPoint(float x, float y)
{
	// invert the y axis
	y *= -1;
	QGraphicsEllipseItem *point = new QGraphicsEllipseItem(x,y,2,2);
	mScene.addItem(point);


	// add the text
	QString t = "(" + QString::number(x) + "," + QString::number(y) + ")";

	QGraphicsTextItem *text = new QGraphicsTextItem(t,point);
	qreal w = text->document()->size().width()/2.0;
	qreal h = text->document()->size().height();
	text->setPos(x - w, y - h);

	// create the line betwen this point and the last
	int l = mPoints.size()-1;
	if(l >= 0){
		QGraphicsEllipseItem *lastPoint = mPoints[l];
		mScene.addLine(lastPoint->rect().x(), lastPoint->rect().y(), x,y);
	}

	// add to the list of points
	mPoints.push_back(point);
}

///////////////////////////////////////////////////////////////////////////////
RealTimeDataDisplayer::RealTimeDataDisplayer(QWidget *parent)
    : QWidget(parent),
      mXMaxValue(-1),
      mYMaxValue(-1),
      mMeasureScale(-1),
      mTimeScale(-1)
{
	ui.setupUi(this);
	ui.graphicsView->setScene(&mScene);

	//mScene.addRect(QRectF(-1000,-1000,20,20));
	//ui.graphicsView->setRenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	//mScene.addRect(QRectF(2010,2010,2000,2000));

	QGraphicsTextItem *t = new QGraphicsTextItem("testing");
	t->setPos(100,100);
	t->setDefaultTextColor(QColor(100,100,100));
	mScene.addItem(t);


	// create the (0,0) point
	QGraphicsEllipseItem *point = new QGraphicsEllipseItem(0,0,2,2);
	mPoints.push_back(point);

}

///////////////////////////////////////////////////////////////////////////////
RealTimeDataDisplayer::~RealTimeDataDisplayer()
{
	// TODO: remove all the scene items
//	QList<QGraphicsItem *> items;
//	items = mScene.items();
//
//	QList<QGraphicsItem *>::iterator it = items.begin();
//	for(; it != items.end(); ++it){
//		delete *it;
//	}

	mScene.clear();

}


///////////////////////////////////////////////////////////////////////////////
void RealTimeDataDisplayer::setMaxYValue(float max)
{
	assert(max > 0);

	if(mYMaxValue != -1){
		std::cerr <<("Warning: Changing the Max Y value 2 times\n");
	}

	mYMaxValue = max;

}

///////////////////////////////////////////////////////////////////////////////
void RealTimeDataDisplayer::setMaxXValue(float max)
{
	assert(max > 0);

	if(mXMaxValue != -1){
		std::cerr <<("Warning: Changing the Max X value 2 times\n");
	}

	mXMaxValue = max;
}

///////////////////////////////////////////////////////////////////////////////
void RealTimeDataDisplayer::setMeasureScale(float s)
{
	assert(s > 0);

	if(mMeasureScale != -1){
		std::cerr << "Warning: Changing the Measure Scale value 2 times\n";
	}

	mMeasureScale = s;

}

///////////////////////////////////////////////////////////////////////////////
void RealTimeDataDisplayer::setTimeScale(float s)
{
	assert(s > 0);

	if(mTimeScale != -1){
		std::cerr << "Warning: Changing the Time Scale value 2 times\n";
	}

	mTimeScale = s;

}
///////////////////////////////////////////////////////////////////////////////
bool RealTimeDataDisplayer::init(void)
{
	createAxis();

	return true;

}
///////////////////////////////////////////////////////////////////////////////
void RealTimeDataDisplayer::addNewPoint(float time, float height)
{
	float x = time * mTimeScale;
	float y = height * mMeasureScale;
	createPoint(x,y);
}
