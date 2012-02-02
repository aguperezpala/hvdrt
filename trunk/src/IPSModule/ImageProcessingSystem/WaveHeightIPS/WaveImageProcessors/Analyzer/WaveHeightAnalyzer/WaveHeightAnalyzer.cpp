/*
 * WaveHeightAnalyzer.cpp
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

#include <cmath>

#ifdef DEBUG
#include <opencv2/highgui/highgui.hpp>
#endif

#include "DebugUtil.h"
#include "WaveHeightAnalyzer.h"


////////////////////////////////////////////////////////////////////////////////
void WaveHeightAnalyzer::createColumnAnalyzers(int numAnalyzers, int middlePoint) const
{
	mColumnAnalyzers.resize(numAnalyzers);
	for(int i = mColumnAnalyzers.size()-1; i>=0; --i){
		mColumnAnalyzers[i].setMiddlePoint(middlePoint);
	}
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightAnalyzer::processColumnAnalyzers(cv::Mat &img) const
{
	// get the left most column
	int column = mTLZone.x;
	for(int i = mColumnAnalyzers.size()-1; i >= 0; --i){
		mColumnAnalyzers[i].configure(img, column, mBRZone.y, mTLZone.y);
		++column;

		// analyze
		mColumnAnalyzers[i].analyze();
	}
	ASSERT(column == mBRZone.x);

	// We will only analyze odd numbers of columns
	int middleColumn;
	if(mColumnAnalyzers.size() % 2 == 0){
		// avoid last column
		middleColumn = (mColumnAnalyzers.size()-1) / 2;
	} else {
		middleColumn = (mColumnAnalyzers.size() / 2);
	}

	static std::vector<ColumnAnalyzerInfo> data;
	if(data.size() != mColumnAnalyzers.size()){
		data.resize(mColumnAnalyzers.size());
	}

	// analyze the middle point
	getHeightAndVariance(mColumnAnalyzers[middleColumn], data[middleColumn]);

	// analyze the other points
	int upperStep, lowerStep;
	for(int step = 1; step <= middleColumn; ++step){
		upperStep = middleColumn + step;
		lowerStep = middleColumn - step;

		getHeightAndVariance(mColumnAnalyzers[upperStep], data[upperStep]);
		getHeightAndVariance(mColumnAnalyzers[lowerStep], data[lowerStep]);
	}

	// get the variance of the middle point, if is bigger than the "umbral"
	// then we check for a better height
	if(data[middleColumn].variance <= MAX_HEIGHT_VARIANCE){
		mLastHeight = data[middleColumn].height;
		return;
	}

	// else we have to get a better wave height

	// now that we have all the info we choose the best option
	mLastHeight = getBetterWaveHeight(data, middleColumn);

	// now correct the possible accumulated error of the ColumnAnalyzers
	int correctColum = mLastHeight * mSizeRelation;

	for(int i = 0; i < mColumnAnalyzers.size(); ++i){
		mColumnAnalyzers[i].correctRelativeHeight(correctColum);
	}

}

////////////////////////////////////////////////////////////////////////////////
float WaveHeightAnalyzer::getBetterWaveHeight(const std::vector<ColumnAnalyzerInfo> &data,
		int middleColumn) const
{
	// get the better height
	float bestVariance = data[middleColumn].variance;
	float bestHeight = data[middleColumn].height;

	float avrgHeight, avrgVariance;
	int upperStep, lowerStep;
	for(int step = 1; step <= middleColumn; ++step){
		upperStep = middleColumn + step;
		lowerStep = middleColumn - step;

		// check the variance of neighbor columns
		if(data[upperStep].variance > bestVariance ||
				data[lowerStep].variance > bestVariance){
			// probably we are getting more erroneus data
			continue;
		}

		// else, this could be a better approximation
		avrgHeight = (data[upperStep].height + data[lowerStep].height) / 2.0f;

		// get the average variance
		avrgVariance = std::abs(bestHeight - avrgHeight);

		if(avrgVariance < bestVariance){
			bestVariance = avrgVariance;
			bestHeight = avrgHeight;
		} // else continue
	}

	// now in best height we have the better height
	return bestHeight;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
WaveHeightAnalyzer::WaveHeightAnalyzer() :
IAnalyzer("WaveHeightAnalyzer"),
mSizeRelation(-1),
mTLZone(-1,-1),
mBRZone(-1,-1),
mTimeStamp(0),
mMiddlePoint(-1),
mLastHeight(-1.0f),
mConfigOk(false),
mPoint(-1,-1)
{

}

////////////////////////////////////////////////////////////////////////////////
WaveHeightAnalyzer::~WaveHeightAnalyzer() {
	// TODO Auto-generated destructor stub
}


////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightAnalyzer::setParameter(int param, double value)
{
	switch(param){
	case TLX_ZONE:
		mTLZone.x = value;
		break;
	case TLY_ZONE:
		mTLZone.y = value;
		break;
	case BRX_ZONE:
		mBRZone.x = value;
		break;
	case BRY_ZONE:
		mBRZone.y = value;
		break;
	case SIZE_RELATION:
		mSizeRelation = value;
		break;
	case POS_WAVE_REPOSE:
		mPoint.y = value;
		break;

	default:
		return INVALID_PARAM;
	}

	if((mTLZone.x != -1) && (mTLZone.y != -1) && (mBRZone.x != -1) &&
			(mBRZone.y != -1) && (mSizeRelation != -1) && (mPoint.y != -1)){
		// configure the middle point
		mMiddlePoint = (mBRZone.x - mTLZone.x) / 2;

		// create the column analyzers
		createColumnAnalyzers((mBRZone.x - mTLZone.x), mPoint.y);

		mConfigOk = true;
	} else {
		mConfigOk = false;
	}

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightAnalyzer::getParameter(int param, double &value) const
{
	switch(param){
	case TLX_ZONE:
		value = mTLZone.x;
		break;
	case TLY_ZONE:
		value = mTLZone.y;
		break;
	case BRX_ZONE:
		value = mBRZone.x;
		break;
	case BRY_ZONE:
		value = mBRZone.y;
		break;
	case SIZE_RELATION:
		value = mSizeRelation;
		break;
	case POS_WAVE_REPOSE:
		value = mMiddlePoint;
		break;

	case GET_TIME:
		value = mLastTime;
		break;
	case GET_HEIGHT:
		value = mLastHeight;
		break;
	case GET_PIXEL_POS:
		value = mPoint.y - (mLastHeight * mSizeRelation);
		break;


	default:
		return INVALID_PARAM;
	}
	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightAnalyzer::processData(cv::Mat &image) const
{
	if(image.channels() != 1){
		debug("Warning: we are assuming that there are only 1 channel\n");
		return INVALID_PARAM;
	}

	if(!mConfigOk){
		debug("Error: The IP was bad configured (incomplete)\n");
		return INCOMPLETE_CONFIGURATION;
	}

	processColumnAnalyzers(image);

	if(!mTimeStamp){
		// we have to create the timestamp
		mTimeStamp = new Timestamp();
	}

	// print the data into the file
	mLastTime = mTimeStamp->getDiffTimestamp();



#ifdef DEBUG
	static bool winCreated = false;
	if(!winCreated){cv::namedWindow("debugWin"); winCreated = true;}

	cv::Point p(image.cols/2, mPoint.y -(mLastHeight * mSizeRelation));
	cv::Scalar c = CV_RGB(255,0,0);

	cv::Mat colorFrame;

	cv::cvtColor(image, colorFrame, CV_GRAY2BGR);
	cv::circle(colorFrame,p , 3, c);
	cv::imshow("debugWin", colorFrame);
	cv::waitKey(200);
#endif

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightAnalyzer::processData(cv::gpu::GpuMat &data) const
{
	return FEATURE_NOT_SUPPORTED;
}
