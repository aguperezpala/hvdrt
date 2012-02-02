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



CurveData::CurveData()
{
	// set the new color to this curve
	if(colorId == COLOR_BLUE){
		mCurve.setPen(QPen(Qt::blue));
		mSpectralCurve.setPen(QPen(Qt::blue));
	} else if(colorId == COLOR_GREEN){
		mCurve.setPen(QPen(Qt::green));
		mSpectralCurve.setPen(QPen(Qt::green));
	} else if(colorId == COLOR_RED){
		mCurve.setPen(QPen(Qt::red));
		mSpectralCurve.setPen(QPen(Qt::red));
	}
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
}


