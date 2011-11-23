/*
 * DataProcessor.h
 *
 *  Created on: 13/11/2011
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

#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include <opencv2/core/core.hpp>

#include <string>
#include <fstream>

#include "DebugUtil.h"
#include "CallBFunctor.h"
#include "CoordsInterpreter.h"
#include "Timestamp.h"

class DataProcessor : public CallBFunctor {
	// Set the max possible variance between one wave height and another. in mm
	static const float		MAX_HEIGHT_VARIANCE		= 20;
public:
	DataProcessor();
	virtual ~DataProcessor();

	/* Set the file name where we will put the data */
	void setFileName(const std::string &fname);

	/* Set the relation between pixels and millimeters.
	 * Requires:
	 * 	@rel		Relation pixels/mm
	 */
	void setRelation(float rel) {ASSERT(rel > 0); mRelation = rel;}

	/* Set the data to analyze */
	void setData(CoordsInterpreter::Data *data);

	/* Set the middle point */
	void setMiddlePoint(const cv::Point &p) {mPoint = p;}

	/* Returns the last height calculated in mm */
	float getLastHeightCalculated(void) const {return mRealLastHeight;}

	/* Returns the last time calculated */
	double getLastTime(void) const {return mLastTime;}

	/* Overloaded function */
	virtual void operator()(void);

private:

	/* Function used to get the height of the wave using the middlePoint and
	 * middlepoint-1 & middlePoint+1 positions. We will get the first value from
	 * bottom to top and check if is a possible value (using the last value
	 * and the left and right values)
	 * Returns:
	 * 	height			The height of the wave.
	 */
	float getWaveHeight(void);

	// auxiliar function used to get the height from the neighbors
	float getNeighborsHeight(void);

private:
	CoordsInterpreter::Data			*mData;
	std::ofstream					mOutFile;
	float							mRelation;
	Timestamp						*mTimeStamp;
	int								mMiddlePoint;
	float							mLastHeight;
	float							mRealLastHeight;
	double							mLastTime;
	cv::Point						mPoint;
};

#endif /* DATAPROCESSOR_H_ */
