/*
 * CurveData.cpp
 *
 *  Created on: 31/01/2012
 *      Author: agustin
 *
 */

#include "CurveData.h"
#include "DebugUtil.h"


using namespace Array;
using namespace fftwpp;

int CurveData::colorId = 0;

////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateFFT(const QVector<double> &xs,
		const QVector<double> &ys, QVector<Complex> &result)
{
	unsigned int n = xs.size();
//	unsigned int np = n/2+1;
	size_t align = sizeof(Complex);

	array1<Complex> f(n,align);
//	array1<Complex> g(n,align);

	fft1d Forward(-1,f);

	// we assume that we have equal intervals time (for all xs[i]-xs[i-i])
	for(unsigned int i=0; i < n; i++) f[i]= Complex(ys[i]);

	Forward.fft(f);

	result.clear();
	for(int i = 0; i < f.Size(); ++i) result.push_back(f[i]);
//	for(int i = 0; i < g.Size(); ++i) result.push_back(g[i]);

}

////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateSpectrum(const QVector<Complex> &Y, QVector<Complex> &result,
		double delta)
{
	double fmin = 1.0 / (Y.size()*delta);
	result.clear();
	int n = Y.size()/2 +1;
	int realSize = Y.size();

	QVector<Complex> YP;
	for(int i = 0; i < n; ++i) YP.push_back(Y[i]);

	// set the YP.*conj(YP) in result.
	for(int i = 0; i < n; ++i) {
		Complex aux = YP[i] * conj(YP[i]);
		aux = sqrt(aux);
		aux *= 2;
		aux /= realSize;
		result.push_back(aux);
	}

	// now get do the last transform
	for(int i = 0; i < n; ++i){
		Complex aux = result[i];
		aux = (aux * aux)/(2*fmin);
		result[i] = aux;
	}
}

////////////////////////////////////////////////////////////////////////////////
double CurveData::getDiscreteInterval(const QVector<double> &xs)
{
	// we assume that all the time is absolute... so we only need to return
	// the maximum time / num_samples
	return xs.back()/static_cast<double>(xs.size());
}


////////////////////////////////////////////////////////////////////////////////
int CurveData::calculateFp(const QVector<Complex> &spectrum)
{
	Complex max = spectrum[0];
	int result = 0;

	for(int i = 1; i < spectrum.size(); ++i){
		if(spectrum[i].real() > max.real()){
			max = spectrum[i];
			result = i;
		}
	}

	return result;
}


////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateHs(const QVector<double> &xs,
		const QVector<double> &ys)
{

}

////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateH(const QVector<double> &xs,
			const QVector<double> &ys)
{

}

////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateTp(const QVector<double> &xs,
			const QVector<double> &ys)
{

}

// Calculate spectral curve
void CurveData::calculateSpectralCurve(double interval,
		const QVector<Complex> &result)
{
	QVector<double> xs;
	QVector<double> ys;

	for(int i = 0; i < result.size(); ++i){
		xs.push_back(i*interval);
		ys.push_back(result[i].real());
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
	double discreteInterval = getDiscreteInterval(xs);

//	mComplexValues.clear();

	QVector<Complex> complx, result;
	// calculate all the values needed
	calculateFFT(xs, ys, complx);
	calculateSpectrum(complx, result, discreteInterval);
	mFp = calculateFp(result);

	debug("discreteInterval: %f\tFp: %d\n",discreteInterval,
			static_cast<int>(mFp*discreteInterval));
	calculateH(xs, ys);
	calculateHs(xs, ys);
	calculateTp(xs, ys);
	calculateSpectralCurve(discreteInterval,result);
	calculateMaxAndMin(xs, ys);

//	mComplexValues.clear();
}


