/*
 * CurveData.cpp
 *
 *  Created on: 31/01/2012
 *      Author: agustin
 *
 */
#include <iostream>
#include <cmath>
#include "CurveData.h"
#include "DebugUtil.h"


using namespace Array;
using namespace fftwpp;

int CurveData::colorId = 0;



// Auxiliar class that will simulate a function given an array of xs and ys values

FuncSimulator::FuncSimulator(const QVector<double> &xs,
			const QVector<double> &ys) : mXs(xs), mYs(ys), mLastIndex(0)
{
	mSize = mXs.size();
	ASSERT(mXs.size() == mYs.size());
}
FuncSimulator::~FuncSimulator()
{

}

double FuncSimulator::operator()(double d) const
{
//	ASSERT(mLastIndex < mXs.size());

	if(mLastIndex >= mSize){return 0.0;}
	ASSERT(mXs[mLastIndex] <= d);

	// get the closest point
	while(mLastIndex < mSize && mXs[mLastIndex] < d) ++mLastIndex;

	if(mLastIndex >= mSize) {
		debug("entro aca\n");
		return mYs[mSize-1];
	}
	// if just what we want returnit
	if(mXs[mLastIndex] == d) return mYs[mLastIndex];

	// else we have to find the better point of this... is an ugly interpolation
	// between the actual point an the next point
//    			(y2 - y1)
//	0 = y - y1 ----------- (x - x1)
//       		(x2 - x1)
	double y2 = mYs[mLastIndex];
	double x2 = mXs[mLastIndex];
	--mLastIndex;
	double y1 = mYs[mLastIndex];
	double x1 = mXs[mLastIndex];

	return y1 + ((y2-y1) / (x2-x1)) * (d - x1);
}






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


//	std::cout << " FFT:G:\n" << g << std::endl;
	result.clear();
//	for(int i = 0; i < f.Size(); ++i) result.push_back(f[i]);
	for(int i = 0; i < g.Size(); ++i) {
		result.push_back(g[i]);
	}

}

////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateSpectrum(const QVector<Complex> &Y, double df,
		QVector<double> &result, int n)
{
//	double fmin = 1.0 / (Y.size()*delta);
//	result.clear();
//	int n = Y.size();
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
//		aux /= static_cast<double>(realSize);
//		result.push_back(aux.real());
//	}
//
//	// now get do the last transform
//	for(int i = 0; i < n; ++i){
//		double aux = result[i];
//		aux = (aux * aux)/(2.0*df);
//		result[i] = aux;
//	}

	double dsize = static_cast<double>( n + 1);
	result.clear();
	debug("n:%d\n", n);
	for(int i = 1; i < Y.size(); ++i){
		// value = A
		double value = (2.0/(dsize))*(std::sqrt(std::pow(Y[i].real(),2.0) +
				std::pow(Y[i].imag(),2.0)));
//		std::cout << "Value: A " << value << "\t";
		// calculate S(f) = A*A/(2*df)
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
	debug("spectrums sizes:%d, %d\n", spectrumxs.size(), spectrumys.size());
	ASSERT(spectrumxs.size() == spectrumys.size());

	for(int i = 2; i < spectrumys.size(); ++i){
		if(spectrumys[i] > max){
			max = spectrumys[i];
			result = i;
		}
	}

	debug("FPcalc[%d]=%f\n", result, spectrumxs[result]);
	mFp = spectrumxs[result];
	return spectrumxs[result];
}


////////////////////////////////////////////////////////////////////////////////
void CurveData::calculateH(const QVector<double> &xs,
			const QVector<double> &ys)
{
//	bool vectorPositive = false;
//	int  numChangeVector = 0;
//	bool lastPositive = false;
//	double max = 0, aux;
//	double lvalue = 0;
//	double seno = 0, cresta = 0;
//	double accumH = 0;
//	int 	numWaves = 0;
//
//	ASSERT(xs.size() == ys.size());
//	ASSERT(ys.size() > 1);
//
//	lvalue = ys[0];
//	vectorPositive = (lvalue > ys[1]) ? false : true;
//	lastPositive = ys[0] > 0.0;
//
//	for(int i = 1; i < ys.size(); ++i){
//		bool auxVecPositive = (lvalue > ys[i]) ? false : true;
//		bool nowPositive = ys[i] > 0.0;
//		std::cout << "nowpositive: " << nowPositive << "\tlastpositive: " <<
//								lastPositive << "\tys[i]:" << ys[i] << std::endl;
//		if(auxVecPositive != vectorPositive){
//			// we have a change... now check what change
//			++numChangeVector;
//
//			if(auxVecPositive){
//				seno = std::abs((ys[i] + lvalue) * 0.5);
//			} else {
//				cresta = std::abs((ys[i] + lvalue) * 0.5);
//			}
//
//			// check if this is the second change of the vector
//			if(numChangeVector == 1){
//				numChangeVector = 0;
//
//				if((lastPositive && !nowPositive) || (!lastPositive && nowPositive)){
//					lastPositive = nowPositive;
//					max = (max > aux) ? max : aux;
//					++numWaves;
//					aux = cresta + seno;
//					accumH += aux;
//					debug("AAAA\n");
//				}
//			}
//		}
//
//		vectorPositive = auxVecPositive;
//	}
//
//
//	debug("numWaveS: %d\taccum: %f\tmax: %f\tseno:%f\tcresta:%f\n", numWaves, accumH, max, seno, cresta);
//
//	mH = accumH / static_cast<double>(numWaves);
//	mHmax = max;
//	mNumWaves = numWaves;
//
//	return;



	////////////////////////////////////////////////////////////////////////////
	double max = 0, aux;
	double lvalue = 0;
	double seno = 0, cresta = 0;
	bool	inNegative = false;
	bool	firstCresta = false;
	double accumH = 0;
	int 	numWaves = 0;
	bool	haveToCalc = false;

	ASSERT(xs.size() == ys.size());
	ASSERT(ys.size() > 1);

	firstCresta = (ys[0] < ys[1]);
	lvalue = ys[0];
	inNegative = (ys[0] < 0.0) ? true : false;
	for(int i = 1; i < ys.size(); ++i){
		if(inNegative){
			// last value was negative...
			if(ys[i] > lvalue && haveToCalc){
				haveToCalc = false;
				// we have a seno
				seno = (ys[i] + lvalue) * 0.5;

				if(firstCresta){
					// we calculate the new H
					aux = std::abs(cresta) + std::abs(seno);
					accumH += aux;
					++numWaves;
					max = (max > aux) ? max : aux;
				} else {
					aux = std::abs(cresta) + std::abs(seno);
					max = (max > aux) ? max : aux;
				}
			}

			if(ys[i] > 0.0){
				haveToCalc = true;
				inNegative = false;
			}
		} else {
			// last value was positive...
			if(ys[i] < lvalue && haveToCalc) {
				haveToCalc =false;
				// we have a cresta
				cresta = (ys[i] + lvalue) * 0.5;
				if(!firstCresta){
					// we have a wave ...
					aux = std::abs(cresta) + std::abs(seno);
					accumH += aux;
					++numWaves;
					max = (max > aux) ? max : aux;
				} else {
					aux = std::abs(cresta) + std::abs(seno);
					max = (max > aux) ? max : aux;
				}
			}
			if(ys[i] < 0.0){
				haveToCalc = true;
				inNegative = true;
			}
		}


		lvalue = ys[i];
	}

	debug("numWaveS: %d\taccum: %f\tmax: %f\tseno:%f\tcresta:%f\n", numWaves, accumH, max, seno, cresta);

	mH = accumH / static_cast<double>(numWaves);
	mHmax = max;
	mNumWaves = numWaves;
}

////////////////////////////////////////////////////////////////////////////////

void CurveData::calculateJONSWAP(const QVector<double> &xs)
{
	QVector<double> rys;

	double gamma = 3.3;
	double alfa = 0.0624 / (0.230+0.0336*gamma-0.185*pow(1.9+gamma,-1.0));
	for(int i = 0; i < xs.size(); ++i){
		double f = xs[i];
		double sigma = (f<= mFp)?0.07 : 0.09;
		double S = alfa * (mHs * mHs)* std::pow(mTp, -2.0) * std::pow(f, -5.0) *
				std::pow(M_E, -1.25 * std::pow(mTp * f, -4.0)) *
				std::pow(gamma, std::pow(M_E,-(std::pow(mTp * f - 1.0, 2.0))/(2.0*sigma*sigma)));

		rys.push_back(S);
	}

	mSpectralJONSWAP.setSamples(xs, rys);

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
	pen.setWidth(1);
	mSpectralCurve.setPen(pen);

	// set the second color
	if(colorId == COLOR_BLUE){
		pen.setColor(Qt::yellow);
	} else if(colorId == COLOR_GREEN){
		pen.setColor(Qt::black);
	} else if(colorId == COLOR_RED){
		pen.setColor(Qt::magenta);
	}
	pen.setWidth(2);
	mSpectralJONSWAP.setPen(pen);


	++colorId;

//
//	// test the integrate
//	QVector<double> xs,ys;
//	for(int i = 0; i < 1000; ++i){
//		double x = static_cast<double>(i)/1000.0;
//		xs.push_back(x);
//		ys.push_back(x*x);
//	}
//	FuncSimulator sim(xs,ys);
//	double result = IntegralCalculator::ApproximateIntegral(sim, 0, 999.0/1000.0, 1000);
//	std::cout << "RESULT INTEGRAL: " << result << std::endl;

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
	calculateSpectrum(complx, df, auxVec, xs.size());
	debug("Fmin(o df): %f\n", df);

	// Calculate fp
	calculateFp(freqVec, auxVec);
	calculateTp();
	debug("Fp: %f\n", mFp);


	// Show the spectral curve
	calculateSpectralCurve(freqVec, auxVec);

	// Calculate all the other values..
	calculateH(xs, ys);
	calculateHs(freqVec, auxVec);


	// Calculate JONSWAP
	calculateJONSWAP(freqVec);

	debug("Tp: %f\tHs: %f\n", mTp, mHs);


//	mFp = calculateFp(result) * freq;
	calculateMaxAndMin(xs, ys);
	double max, min;
	getMaxMin(max,min, ys);

	debug("Max calculated: %f\t Min calculated: %f\n", max,min);

//	mComplexValues.clear();
}


