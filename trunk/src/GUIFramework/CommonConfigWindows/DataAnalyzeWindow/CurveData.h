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

	// Returns the different fields
	double getHs(void) const {return mHs;}
	double getH(void) const {return mH;}
	double getTp(void) const {return mTp;}

	// get maximums / minimums
	double getMaxTime(void) const {return mMaxTime;}
	double getMinTime(void) const {return mMinTime;}
	double getMaxHeight(void) const {return mMaxHeight;}
	double getMinHeight(void) const {return mMinHeight;}


private:

	void calculateFFT(const QVector<double> &xs,
			const QVector<double> &ys, QVector<Complex> &result);

	void calculateSpectrum(const QVector<Complex> &Y, double df,
			QVector<double> &result);

	// total_time/total_num_samples and dfmin
	void calculateVecFreq(const QVector<double> &xs, QVector<double> &result,
			double &df);



	// calcualte the fp
	double calculateFp(const QVector<double> &spectrumxs, const QVector<double> &spectrumys);

	// Calculate Hs;
	void calculateHs(const QVector<double> &ys)
	{
		// Hs = 4.004 * sqrt(m0), where m0 = total process variance
		mHs = 4.004 * sqrt(calculateSqrVariance(ys));
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

	int				mFp;
	double			mHs;
	double 			mH;
	double			mTp;
	QwtPlotCurve	mCurve;
	QwtPlotCurve	mSpectralCurve;
//	std::vector<fftw_complex>	mComplexValues;


	static int colorId;
};

#endif /* CURVEDATA_H_ */
