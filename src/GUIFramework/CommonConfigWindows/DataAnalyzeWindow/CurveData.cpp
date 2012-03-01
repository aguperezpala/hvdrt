/*
 * CurveData.cpp
 *
 *  Created on: 31/01/2012
 *      Author: agustin
 *
 */
#include <cmath>
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
	unsigned int np = n/2+1;
	size_t align = sizeof(Complex);

	array1<double> f(n,align);
	array1<Complex> g(np,align);

	rcfft1d Forward(n,f,g);
	crfft1d Backward(n,g,f);

	for(unsigned int i=0; i < n; i++) f[i]=ys[i];

	Forward.fft(f,g);


//	std::cout << " G:\n" << g << std::endl;
	result.clear();
//	for(int i = 0; i < f.Size(); ++i) result.push_back(f[i]);
	for(int i = 0; i < g.Size(); ++i) {
		result.push_back(g[i]);
	}

}

////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateSpectrum(const QVector<Complex> &Y, double df,
		QVector<double> &result)
{
//	double fmin = 1.0 / (Y.size()*delta);
//	result.clear();
//	int n = Y.size()/2 +1;
//	int realSize = Y.size();
//
//	QVector<Complex> YP;
//	for(int i = 0; i < n; ++i) YP.push_back(Y[i]);
//
//	// set the YP.*conj(YP) in result.
//	for(int i = 0; i < n; ++i) {
//		Complex aux = YP[i] * conj(YP[i]);
//		aux = sqrt(aux);
//		aux *= 2;
//		aux /= realSize;
//		result.push_back(aux);
//	}
//
//	// now get do the last transform
//	for(int i = 0; i < n; ++i){
//		Complex aux = result[i];
//		aux = (aux * aux)/(2*fmin);
//		result[i] = aux;
//	}

	result.clear();
	for(int i = 1; i < Y.size(); ++i){
		double value = (std::sqrt(std::pow(Y[i].real(),2.0) +
				std::pow(Y[i].imag(),2.0)));
		value = value * value / (2.0*df);
		result.push_back(value);
	}
}

////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateVecFreq(const QVector<double> &xs, QVector<double> &result,
		double &df)
{
	// get the sample rate
	double sampleRate = xs.back()/static_cast<double>(xs.size());
	int halfSize = (xs.size()/2.0)+1;
	// calculate the fMin = df = 1/(N*dt). where dt = sampleRate
	double step = 1.0/(static_cast<double>(xs.size())*sampleRate);
//	double step = 3.0/static_cast<double>(xs.size());
	df = step;



	// create the freqVector
	result.clear();
	for(int i = 1; i < halfSize; ++i){
		result.push_back(static_cast<double>(i)*step);
	}

	debug("num samples (halfsize): %d, sample rate: %f, step: %f\n",
			halfSize, sampleRate, step);
}

////////////////////////////////////////////////////////////////////////////////
double CurveData::calculateFp(const QVector<double> &spectrumxs,
		const QVector<double> &spectrumys)
{
	double max = spectrumys[1];
	int result = 0;
	ASSERT(spectrumxs.size() == spectrumys.size());

	for(int i = 2; i < spectrumys.size(); ++i){
		if(spectrumys[i] > max){
			max = spectrumys[i];
			result = i;
		}
	}

	debug("FPcalc[%d]=%f\n", result, spectrumxs[result]);
	return spectrumxs[result];
}


////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateH(const QVector<double> &xs,
			const QVector<double> &ys)
{

}

// Calculate spectral curve
void CurveData::calculateSpectralCurve(const QVector<double> &freqVec,
		const QVector<double> &result)
{
////	QVector<double> xs;
//	QVector<double> ys;
//
//	for(int i = 0; i < result.size(); ++i){
////		xs.push_back(i*freq);
////		xs.push_back(result[i].imag());
//		ys.push_back(result[i].real());
//	}
	mSpectralCurve.setSamples(freqVec, result);

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


	QVector<double> freqVec, auxVec;
	QVector<Complex> complx;
	double df;


	// calculate freq vector
	calculateVecFreq(xs,freqVec, df);

	// Apply FFT
	calculateFFT(xs, ys, complx);

	// Now calculate the spectrum
	calculateSpectrum(complx, df, auxVec);

	// Calculate fp
	mFp = calculateFp(freqVec, auxVec);
	debug("Fp: %f\n", mFp);


	// Show the spectral curve
	calculateSpectralCurve(freqVec, auxVec);

	// Calculate all the other values..
	calculateH(xs, ys);
	calculateHs(auxVec);
	calculateTp();

	debug("Tp: %f\tHs: %f\n", mTp, mHs);


//	mFp = calculateFp(result) * freq;
	calculateMaxAndMin(xs, ys);
	double max, min;
	getMaxMin(max,min, ys);
	debug("Max calculated: %f\t Min calculated: %f\n", max,min);

//	mComplexValues.clear();
}


