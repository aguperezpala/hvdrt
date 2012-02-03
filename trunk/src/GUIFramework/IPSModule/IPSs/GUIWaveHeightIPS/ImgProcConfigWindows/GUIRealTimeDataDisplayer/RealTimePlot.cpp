/*
 * RealTimePlot.cpp
 *
 *  Created on: 02/02/2012
 *      Author: agustin
 *
 */
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>

#include "DebugUtil.h"
#include "RealTimePlot.h"


////////////////////////////////////////////////////////////////////////////////
RealTimePlot::RealTimePlot(QWidget *parent):
QwtPlot(parent),
mTimerId(-1),
mMaxY(-99999999.0),
mMinY(99999999.0),
mDirtyAxis(false)
{
	// insert grid
	mGrid.attach(this);

	// insert curve
	mCurve.setPen(QPen(Qt::blue));
	mCurve.setData(&mCircularBuff);
	mCurve.attach(this);

	 // Axis
	setAxisTitle(QwtPlot::xBottom, "segundos");
	setAxisScale(QwtPlot::xBottom, 0, mCircularBuff.getCantSeconds());

	setAxisTitle(QwtPlot::yLeft, "metros");
	setAxisScale(QwtPlot::yLeft, -0.4, 0.4);
//	setAxisAutoScale(QwtPlot::yLeft, true);


}

////////////////////////////////////////////////////////////////////////////////
RealTimePlot::~RealTimePlot()
{
	stopRefresh();
}


////////////////////////////////////////////////////////////////////////////////
void RealTimePlot::timerEvent(QTimerEvent *e)
{
	canvas()->replot();

//	if(mDirtyAxis){
//		setAxisScale(QwtPlot::yLeft, mMinY, mMaxY);
//		replot();
//		mDirtyAxis = false;
//	} else {
//		canvas()->replot();
//	}

}

void RealTimePlot::clearData(void)
{
	mMaxY = -999999999.0;
	mMinY = 99999999.0;
	mCircularBuff.clear();
}

void RealTimePlot::addNewPoint(double x, double y)
{
//	if(mMaxY < y){
//		mMaxY = y;
//		if(mMinY > y){
//			mMinY = y;
//		}
//		// change the scales
//		mDirtyAxis = true;
//	} else if(mMinY > y){
//		mMinY = y;
//		mDirtyAxis = true;
//	}

	mCircularBuff.addNewPoint(x,y);
//	debug("NewPoint: y: %lf, maxY: %lf, minY: %lf\n", y, mMaxY, mMinY);
}

void RealTimePlot::startRefresh(void)
{
	mTimerId = startTimer(TIMER_REFRESH_TIME);
}

void RealTimePlot::stopRefresh(void)
{
	if(mTimerId >= 0){
		killTimer(mTimerId);
		mTimerId = -1;
	}
}
