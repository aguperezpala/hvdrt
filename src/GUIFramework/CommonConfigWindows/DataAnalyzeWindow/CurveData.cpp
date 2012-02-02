/*
 * CurveData.cpp
 *
 *  Created on: 31/01/2012
 *      Author: agustin
 *
 */

#include "CurveData.h"
#include "DebugUtil.h"


int CurveData::colorId = 0;

// Calculate Hs;
void CurveData::calculateHs(const QVector<double> &xs,
		const QVector<double> &ys)
{

}

// calculate H
void CurveData::calculateH(const QVector<double> &xs,
			const QVector<double> &ys)
{

}

// calculate Tp
void CurveData::calculateTp(const QVector<double> &xs,
			const QVector<double> &ys)
{

}

// Calculate spectral curve
void CurveData::calculateSpectralCurve(const QVector<double> &xs,
				const QVector<double> &ys)
{

}

// calculate maximums and minimums
void CurveData::calculateMaxAndMin(const QVector<double> &xs,
				const QVector<double> &ys)
{
	ASSERT(!xs.empty());
	ASSERT(!ys.empty());

	mMaxHeight = mMinHeight = ys[0];
	mMaxTime = mMinTime = xs[0];

	for(int i = xs.size()-1; i >= 0; --i){
		if(mMaxHeight < ys[i]) mMaxHeight = ys[i];
		if(mMinHeight > ys[i]) mMinHeight = ys[i];

		if(mMaxTime < xs[i]) mMaxTime = xs[i];
		if(mMinTime > xs[i]) mMinTime = xs[i];

	}
}

CurveData::CurveData()
{
	// set the new color to this curve
	QPen pen;
	if(colorId == COLOR_BLUE){
		pen.setColor(Qt::blue);
	} else if(colorId == COLOR_GREEN){
		pen.setColor(Qt::green);
	} else if(colorId == COLOR_RED){
		pen.setColor(Qt::red);
	}

	pen.setWidth(2);
	mCurve.setPen(pen);
	mSpectralCurve.setPen(pen);


	++colorId;

}

CurveData::~CurveData()
{
	mCurve.detach();
	mSpectralCurve.detach();
}



/* Load data from vectors */
void CurveData::loadData(const QVector<double> &xs,
			const QVector<double> &ys)
{
	ASSERT(xs.size() == ys.size());
	mCurve.setSamples(xs, ys);

	// calculate all the values needed
	calculateH(xs, ys);
	calculateHs(xs, ys);
	calculateTp(xs, ys);
	calculateSpectralCurve(xs, ys);
	calculateMaxAndMin(xs, ys);
}


