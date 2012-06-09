/*
 * CurveData.h
 *
 *  Created on: 31/01/2012
 *      Author: agustin
 *
 */

#ifndef CURVEDATA_H_
#define CURVEDATA_H_

#include <qvector.h>
#include <qwt_plot_curve.h>
#include "Array.h"
#include "fftw++.h"
#include "DebugUtil.h"
#include "IntegralCalculator.h"



// Auxiliar class that will simulate a function given an array of xs and ys values
class FuncSimulator : public FunctionFunctr {
public:
	FuncSimulator(const QVector<double> &xs,
				const QVector<double> &ys);
	~FuncSimulator();

	double operator()(double d) const;

private:
	const QVector<double> &mXs;
	const QVector<double> &mYs;
	int						mSize;
	mutable int				mLastIndex;

};


class CurveData {
public:
	CurveData();
	virtual ~CurveData();

	/* Load data from vectors */
	void loadData(const QVector<double> &xs,
				const QVector<double> &ys);

	// Returns the curve associated to the data
	QwtPlotCurve &getDataCurve(void){return mCurve;}

	// returns the spectral curve
	QwtPlotCurve &getSpectralCurve(void){return mSpectralCurve;}

	QwtPlotCurve &getSpectralJONSWAP(void){return mSpectralJONSWAP;}

	// Returns the different fields
	double getHs(void) const {return mHs;}
	double getHmax(void) const {return mHmax;}
	double getH(void) const {return mH;}
	int getNumwaves(void) const {return mNumWaves;}

	double getTp(void) const {return mTp;}

	// get maximums / minimums
	double getMaxTime(void) const {return mMaxTime;}
	double getMinTime(void) const {return mMinTime;}
	double getMaxHeight(void) const {return mMaxHeight;}
	double getMinHeight(void) const {return mMinHeight;}

	/**
	 * Clear the colors
	 */
	static void clearColors(void);

private:

	void calculateFFT(const QVector<double> &xs,
			const QVector<double> &ys, QVector<Complex> &result);

	void calculateSpectrum(const QVector<Complex> &Y, double df,
			QVector<double> &result, int n);

	// total_time/total_num_samples and dfmin
	void calculateVecFreq(const QVector<double> &xs, QVector<double> &result,
			double &df);

	// calcualte the fp
	double calculateFp(const QVector<double> &spectrumxs, const QVector<double> &spectrumys);

	// Calculate Hs;
	void calculateHs(const QVector<double> &xs, const QVector<double> &ys)
	{
		// Hs = 4.004 * sqrt(m0), where m0 = total process variance
//		mHs = 4.004 * sqrt(calculateSqrVariance(ys));
		FuncSimulator fsim(xs, ys);
		double start = xs[0];
		double end = xs.back();

		mHs = IntegralCalculator::ApproximateIntegral(fsim, start, end, 10000);
		mHs = 4.004 * std::sqrt(mHs);
	}

	// calculate H
	void calculateH(const QVector<double> &xs,
				const QVector<double> &ys);

	// calculate Tp
	void calculateTp()
	{
		mTp = 1.0/mFp;
	}

	double calculateMean(const QVector<double> &xs)
	{
		double mean = 0.0;
		for(int i = 0; i < xs.size(); ++i){
			mean += xs[i];
		}
		return mean/static_cast<double>(xs.size());
	}

	double calculateSqrVariance(const QVector<double> &xs)
	{
		double mean = calculateMean(xs);
		double sqrVar = 0.0;
		for(int i = 0; i < xs.size(); ++i){
			sqrVar += std::pow(xs[i] - mean,2.0);
		}
		sqrVar /= static_cast<double>(xs.size());
		debug("Mean: %f\t sqrVar: %f\n", mean, sqrVar);
		return sqrVar;
	}

	void getMaxMin(double &max, double &min, const QVector<double> &v)
	{
		max = min = v[0];
		for(int i = 0; i < v.size(); ++i){
			if(max < v[i]) max = v[i];
			if(min > v[i]) min = v[i];
		}
	}


	// calculate JONSWAP
	void calculateJONSWAP(const QVector<double> &freqVec);


	// Calculate spectral curve
	void calculateSpectralCurve(const QVector<double> &freqVec,
			const QVector<double> &result);

	// calculate maximums and minimums
	void calculateMaxAndMin(const QVector<double> &xs,
					const QVector<double> &ys);


	enum {
		COLOR_GREEN,
		COLOR_RED,
		COLOR_BLUE
	};
private:
	double			mMaxTime;
	double			mMinTime;
	double			mMaxHeight;
	double			mMinHeight;

	double			mFp;
	double			mHs;
	double 			mH;
	double 			mHmax;
	int				mNumWaves;
	double			mTp;
	QwtPlotCurve	mCurve;
	QwtPlotCurve	mSpectralCurve;
	QwtPlotCurve	mSpectralJONSWAP;
//	std::vector<fftw_complex>	mComplexValues;


	static int colorId;
};

#endif /* CURVEDATA_H_ */
