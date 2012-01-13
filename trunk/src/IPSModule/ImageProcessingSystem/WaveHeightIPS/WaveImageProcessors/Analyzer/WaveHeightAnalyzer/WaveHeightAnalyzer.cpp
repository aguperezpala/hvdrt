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

#include "DebugUtil.h"
#include "WaveHeightAnalyzer.h"

////////////////////////////////////////////////////////////////////////////////
void WaveHeightAnalyzer::clearDataVec(void) const
{
	for(int i = mData.size()-1; i >= 0; --i){
		mData.at(i).clear();
	}
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightAnalyzer::postProcessData(void) const
{
	if(!mTimeStamp){
		// we have to create the timestamp
		mTimeStamp = new Timestamp();
	}

	// now process the data
	mLastHeight = getWaveHeight();

	// now transform the high using the middle point
	mRealLastHeight = mLastHeight - (mPoint.y / mSizeRelation);

	// print the data into the file
	mLastTime = mTimeStamp->getDiffTimestamp();

}

////////////////////////////////////////////////////////////////////////////////
float WaveHeightAnalyzer::getNeighborsHeight(void) const
{
	int neighborHeight = 0;
	// we have not detected the new size, try getting the average from the
	// pixels of the sides
	std::vector<int> &l = mData.at(mMiddlePoint-1);
	std::vector<int> &r = mData.at(mMiddlePoint+1);
	int lh = 0;
	int rh = 0;
	if(l.size() != 0) {
		// no left pixel
		lh = l[l.size()-1];
	}
	if(r.size() != 0){
		rh = r[r.size()-1];
	}

	// get the neighbor height
	if(rh != 0 && lh != 0){
		neighborHeight = (rh+lh)/2;
	} else if (rh != 0){
		neighborHeight = rh;
	} else if (lh != 0){
		neighborHeight = lh;
	}

	// now check if the neighborHeight is possible
	float mmh = neighborHeight / mSizeRelation;
	if(mLastHeight > 0.0f){
		if(std::abs(mmh-mLastHeight) < MAX_HEIGHT_VARIANCE){
			// we have a correct value, return it
			return mmh;
		} else {
			// we cannot detect the value, return the last height
			debug("Warning: Coludn't detect the height, returning the last"
					" calculated Height\n");
			return mLastHeight;
		}
	} else {
		// we can see if we have some "normal value"
		if(mmh == 0){
			debug("Warning: Problem detecting the height\n");
			return mLastHeight;
		} else {
			return mmh;
		}
	}
}

/* Function used to get the height of the wave using the middlePoint and
 * middlepoint-1 & middlePoint+1 positions. We will get the first value from
 * bottom to top and check if is a possible value (using the last value
 * and the left and right values)
 * Returns:
 * 	height			The height of the wave.
 */
////////////////////////////////////////////////////////////////////////////////
float WaveHeightAnalyzer::getWaveHeight(void) const
{
	ASSERT(mMiddlePoint > 0);

	//TODO: Aca no estamos trabajando con mas de un valor, lo que deberiamos
	// hacer es tener en cuenta el valor anterior y hacia donde estaba yendo la ola
	// (vector de direccion) para poder estimar aproximadamente la nueva posicion
	// y de esta forma obtener el valor que se encuentra lo mas proximo a
	// lastPosition + moveDirectionVector (simplemente con std::abs() con cada
	// valor para obtener el mas cercano).
	//


	// now get the first point from bottom to top
	std::vector<int> &p = mData.at(mMiddlePoint);
	if(p.size() == 0){
		return getNeighborsHeight();
	}
	// else we have some value, try to get the height
	int h = p[p.size()-1];
	if(mLastHeight > 0){
		float mmh = h / mSizeRelation;// TODO: save 1/Relation and multiply here (is faster)
		if(std::abs(h - mLastHeight) < MAX_HEIGHT_VARIANCE){
			// we have a good value
			return mmh;
		} else {
			// to much variance, try to approximate using the neighbors
			return getNeighborsHeight();
		}
	} else {
		float mmh = h / mSizeRelation;
		if(mmh == 0.0f){
			return getNeighborsHeight();
		} else {
			return mmh;
		}
	}

	ASSERT(false);
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
mConfigOk(false)
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
		mMiddlePoint = value;
		break;

	default:
		return INVALID_PARAM;
	}

	if((mTLZone.x != -1) && (mTLZone.y != -1) && (mBRZone.x != -1) &&
			(mBRZone.y != -1) && (mSizeRelation != -1)){

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
		value = mRealLastHeight;
		break;
	case GET_PIXEL_POS:
		debug("Warning: trying to get an unused feature\n");
		return FEATURE_NOT_SUPPORTED;
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

	// Clears the old data
	clearDataVec();

	// set the coords to analyze
	int l = mTLZone.y;
	int nl = l + (mBRZone.y - mTLZone.y);

	int c = mTLZone.x;
	int beginColumn = mTLZone.x;
	int nc = c + (mBRZone.x - mTLZone.x);

	int brY = mBRZone.y;

	// Check that we are inside of the image
	if((nl > image.rows) || (c  > image.cols)){
		debug("Error: Analyze Zone is wrong\n");
		ASSERT(false);
	}

	uchar* data = image.data + l*image.step + c*image.elemSize();
	// for all pixels
	for (; l < nl; ++l) {
		for (c = beginColumn; c < nc; ++c){
			if (*data == 0) {
				// go to the next column
				data += image.elemSize();
				continue;
			}
			// we have a point to store
			(mData.at(c - beginColumn)).push_back(brY - l);
			data += image.elemSize();
		}

		// go to the next row/line
		data = image.data + l*image.step + beginColumn*image.elemSize();
	}

	// TODO: post process the data
	postProcessData();

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightAnalyzer::processData(cv::gpu::GpuMat &data) const
{
	return FEATURE_NOT_SUPPORTED;
}
