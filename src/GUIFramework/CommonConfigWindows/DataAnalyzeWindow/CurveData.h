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
			const QVector<double> &ys);

	// Calculate Hs;
	void calculateHs(const QVector<double> &xs,
			const QVector<double> &ys);

	// calculate H
	void calculateH(const QVector<double> &xs,
				const QVector<double> &ys);

	// calculate Tp
	void calculateTp(const QVector<double> &xs,
				const QVector<double> &ys);

	// Calculate spectral curve
	void calculateSpectralCurve(void);

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

	double			mHs;
	double 			mH;
	double			mTp;
	QwtPlotCurve	mCurve;
	QwtPlotCurve	mSpectralCurve;
	std::vector<fftw_complex>	mComplexValues;


	static int colorId;
};

#endif /* CURVEDATA_H_ */
