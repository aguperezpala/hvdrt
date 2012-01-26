/*
 * WaveHeightAnalyzer.h
 *
 *  Created on: 03/01/2012
 *      Author: agustin
 *
 *
 * OpenVC License
 *
 * Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
 * Copyright (C) 2009, Willow Garage Inc., all rights reserved.
 * Third party copyrights are property of their respective owners.
 *
 * This software is provided by the copyright holders and contributors "as is" and
 * any express or implied warranties, including, but not limited to, the implied
 * warranties of merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the Intel Corporation or contributors be liable for any direct,
 * indirect, incidental, special, exemplary, or consequential damages
 * (including, but not limited to, procurement of substitute goods or services;
 * loss of use, data, or profits; or business interruption) however caused
 * and on any theory of liability, whether in contract, strict liability,
 * or tort (including negligence or otherwise) arising in any way out of
 * the use of this software, even if advised of the possibility of such damage.
 */

#ifndef WAVEHEIGHTANALYZER_H_
#define WAVEHEIGHTANALYZER_H_

#include <opencv2/core/core.hpp>

#include <vector>

#include "Timestamp.h"
#include "IAnalyzer.h"
#include "ColumnAnalyzer.h"

class WaveHeightAnalyzer : public IAnalyzer {

	// Set the max possible variance between one wave height and another. in mm
	static const float		MAX_HEIGHT_VARIANCE		= 20;

public:
	enum {
		// Analyze zone
		TLX_ZONE = IAnalyzer::GET_PIXEL_POS+1,
		TLY_ZONE,
		BRX_ZONE,
		BRY_ZONE,
		SIZE_RELATION,	// the relation between pixels and millimeters (pixels/mm)
		POS_WAVE_REPOSE,// the position where the wave is in repose
	};

public:
	WaveHeightAnalyzer();
	virtual ~WaveHeightAnalyzer();

	// Set a parameter to the ImageProcessor, used to configure the IP.
	errCode setParameter(int param, double value);

	// Gets a parameter value returned by param
	errCode getParameter(int param, double &value) const;

	// Proccess the data on the CPU
	errCode processData(cv::Mat &data) const;

	// Process the data on the GPU
	errCode processData(cv::gpu::GpuMat &data) const;

private:
	// Define a structure used to store the height and variance of a column analyzer
	typedef struct {
		float	height;
		float	variance;
		float	previusHeight;
	} ColumnAnalyzerInfo;


private:
	// Create the column analyzers
	void createColumnAnalyzers(int numAnalyzers, int middlePoint) const;

	// configure and process the data of the ColumnAlayzers
	void processColumnAnalyzers(cv::Mat &img) const;

	// This function retrieves the height in mm from a ColumnAnalyzer and the
	// variance between the actual point and the last point calculated.
	inline void getHeightAndVariance(const ColumnAnalyzer &ca,
			ColumnAnalyzerInfo &cai) const
	{
		cai.height = static_cast<float>(ca.getRelativeLastHeight()) / mSizeRelation;
		cai.variance = static_cast<float>(ca.getRelativePreviusHeight()) / mSizeRelation;
		cai.variance = std::abs(cai.variance - cai.height);
		cai.previusHeight = static_cast<float>(ca.getRelativePreviusHeight()) / mSizeRelation;
	}

	// Function that analyze a vector of ColumnAnalyzerInfo and gets the best
	// wave height.
	float getBetterWaveHeight(const std::vector<ColumnAnalyzerInfo> &data,
			int middleColumn) const;

private:

	typedef std::vector<ColumnAnalyzer>		ColumnAnalyzerVec;


	double							mSizeRelation;
	cv::Point2i						mTLZone;
	cv::Point2i						mBRZone;
	// Data processor
	mutable Timestamp				*mTimeStamp;
	int								mMiddlePoint;
	mutable float					mLastHeight;
	mutable double					mLastTime;
	cv::Point						mPoint;

	bool							mConfigOk;

	mutable ColumnAnalyzerVec		mColumnAnalyzers;

};

#endif /* WAVEHEIGHTANALYZER_H_ */
