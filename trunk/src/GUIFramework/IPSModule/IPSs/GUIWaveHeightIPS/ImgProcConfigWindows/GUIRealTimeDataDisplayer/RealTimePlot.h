/*
 * RealTimePlot.h
 *
 *  Created on: 02/02/2012
 *      Author: agustin
 *
 */

#ifndef REALTIMEPLOT_H_
#define REALTIMEPLOT_H_

#include <qwt_plot.h>
#include <qwt_system_clock.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

#include "CircularBuffer.h"


class RealTimePlot : QwtPlot
{
	 Q_OBJECT

	 static const int TIMER_REFRESH_TIME	=	33; // refresh 30 times per second
public:
	RealTimePlot(QWidget*w = NULL);
	virtual ~RealTimePlot();

	void clearData(void);

	void startRefresh(void);
	void stopRefresh(void);

protected:
    virtual void timerEvent(QTimerEvent *e);

private:
    double 			mInterval;
    CircularBuffer	mCircularBuff;
    QwtPlotCurve	mCurve;
    QwtPlotGrid		mGrid;
    int				mTimerId;
};

#endif /* REALTIMEPLOT_H_ */
