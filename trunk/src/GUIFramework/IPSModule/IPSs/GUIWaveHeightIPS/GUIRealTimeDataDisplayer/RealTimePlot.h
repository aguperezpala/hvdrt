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


class RealTimePlot : public QwtPlot
{
	 Q_OBJECT

	 static const int TIMER_REFRESH_TIME	=	250; // refresh 30 times per second
public:
	RealTimePlot(QWidget *w = 0);
	virtual ~RealTimePlot();

	void clearData(void);

	void addNewPoint(double x, double y);

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
    double			mMaxY;
    double			mMinY;
    bool			mDirtyAxis;
};

#endif /* REALTIMEPLOT_H_ */
