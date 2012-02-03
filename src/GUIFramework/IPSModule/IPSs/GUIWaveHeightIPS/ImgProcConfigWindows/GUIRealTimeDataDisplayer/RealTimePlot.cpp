/*
 * RealTimePlot.cpp
 *
 *  Created on: 02/02/2012
 *      Author: agustin
 *
 */

#include "RealTimePlot.h"


////////////////////////////////////////////////////////////////////////////////
RealTimePlot::RealTimePlot(QWidget *parent):
QwtPlot(parent),
mTimerId(-1)
{
	// insert grid
	mGrid.attach(this);

	// insert curve
	mCurve.setPen(QPen(Qt::blue));
	mCurve.setData(&mCircularBuff);
	mCurve.attach(this);

	 // Axis
	setAxisTitle(QwtPlot::xBottom, "segundos");
	setAxisScale(QwtPlot::xBottom, 0, 10.0);

	setAxisTitle(QwtPlot::yLeft, "metros");
	setAxisScale(QwtPlot::yLeft, -0.7, 0.7);

}

////////////////////////////////////////////////////////////////////////////////
RealTimePlot::~RealTimePlot()
{
	stopRefresh();
}


////////////////////////////////////////////////////////////////////////////////
void RealTimePlot::timerEvent(QTimerEvent *e)
{
//	 CircularBuffer *buffer = (CircularBuffer *)d_curve->data();
	// buffer->setReferenceTime(mClock.elapsed() / 1000.0);

//
//	switch(d_settings.updateType)
//	{
//		case Settings::RepaintCanvas:
//		{
//			// the axes in this example doesn't change. So all we need to do
//			// is to repaint the canvas.
//
//			canvas()->replot();
//			break;
//		}
//		default:
//		{
//			replot();
//		}
//	}
}

void RealTimePlot::clearData(void)
{
	mCircularBuff.clear();
}

void RealTimePlot::startRefresh(void)
{
	mTimerId = startTimer(TIMER_REFRESH_TIME);
}

void RealTimePlot::stopRefresh(void)
{
	if(mTimerId >= 0){
		killTimer(mTimerId);
	}
}
