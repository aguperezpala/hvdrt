/*
 * CurveData.cpp
 *
 *  Created on: 31/01/2012
 *      Author: agustin
 *
 */

#include "CurveData.h"
#include "DebugUtil.h"
#include <fftw3.h>


int CurveData::colorId = 0;


// create the complex data vector
void CurveData::createComplexVector(const QVector<double> &xs,
		const QVector<double> &ys)
{
	for(int i = 0; i < xs.size(); ++i){
		mComplexValues.push_back(complex(xs[i]+10, ys[i]+10));
	}
}

void CurveData::calculateFFT(const QVector<double> &xs,
		const QVector<double> &ys)
{
	fftw_complex *in;
	fftw_complex *out;
	fftw_plan p;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * xs.size());
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * xs.size());


	for(int i = 0; i < mComplexValues.size(); ++i) out[i] = mComplexValues[i];
	if(!CFFT::Forward(out, mComplexValues.size())){
		debug("Error calculating the FFT\n");
		 fftw_free(in); fftw_free(out);
		return;
	}

	int i = mComplexValues.size();
	mComplexValues.clear();
	for(int j = 0; j < i; ++j){
		mComplexValues.push_back(out[i]);
	}

	 fftw_free(in); fftw_free(out);
}


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
void CurveData::calculateSpectralCurve(void)
{
	QVector<double> xs;
	QVector<double> ys;

	for(int i = 0; i < mComplexValues.size(); ++i){
		xs.push_back(mComplexValues[i].re());
		ys.push_back(mComplexValues[i].im());
	}
	mSpectralCurve.setSamples(xs, ys);

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

	mComplexValues.clear();

	// calculate all the values needed
	calculateFFT(xs, ys);
	calculateH(xs, ys);
	calculateHs(xs, ys);
	calculateTp(xs, ys);
	calculateSpectralCurve();
	calculateMaxAndMin(xs, ys);

	mComplexValues.clear();
}


