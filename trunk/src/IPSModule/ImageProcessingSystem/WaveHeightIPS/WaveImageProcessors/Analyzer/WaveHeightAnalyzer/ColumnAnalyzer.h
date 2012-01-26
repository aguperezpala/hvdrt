/*
 * ColumnAnalyzer.h
 *
 *  Created on: 26/01/2012
 *      Author: agustin
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


#ifndef COLUMNANALYZER_H_
#define COLUMNANALYZER_H_


#include <opencv2/core/core.hpp>

#include "DebugUtil.h"


class ColumnAnalyzer {
public:
	ColumnAnalyzer();
	virtual ~ColumnAnalyzer();

	// Set the middle point (number of row where the middlePoint is)
	void setMiddlePoint(size_t middle);

	// Configure the ColumnAnalyzer from an cv::Mat and a num of column to analyze
	void configure(cv::Mat &m, size_t numColumn, size_t topRow, size_t bottomRow);

	// Configure using values
	void configure(size_t topRow, size_t bottomRow,
			uchar *baseAddr, size_t jumpSize);

	// Analyze the new wave height in pixels
	void analyze(void);

	// Correct the actual height
	void correctRelativeHeight(int h);

	// Returns the last height analyzed. This height is the height counting
	// from the base row (bottom row).
	inline int getLastHeight(void) const
	{
		// Transforms the height to the bottom-top coords system
		return mTopRow - mLastHeight;
	}

	// Returns a transformed height using the middlepoint row (relative to this)
	inline int getRelativeLastHeight(void) const
	{
		// first we transform to the bottom-top "row" coordinates
		// now we get the middle point and get the relative pos
		return (mMiddlePoint - mLastHeight);
	}

	// Returns the PREVIUS height analyzed. This height is the height counting
	// from the base row (bottom row).
	inline int getPreviusHeight(void) const
	{
		// Transforms the height to the bottom-top coords system
		return mTopRow - mPreviusHeight;
	}

	// Returns a transformed PREVIUS height using the middlepoint row (relative to this)
	inline int getRelativePreviusHeight(void) const
	{
		// first we transform to the bottom-top "row" coordinates
		// now we get the middle point and get the relative pos
		return (mMiddlePoint - mPreviusHeight);
	}


	// Returns the middle point
	inline size_t getMiddlePoint(void) const {return mMiddlePoint;}



private:
	// Check if a pixel is set or not given a position (row) on this column
	inline bool isPixelSet(size_t pos)
	{
		if(!(pos < mTopRow)){
			debug("pos: %d\tmTopRow: %d\n", pos, mTopRow);
		}
		ASSERT(pos < mTopRow);
		uchar *data = mBaseAddr + pos * mRowJumpSize;
		return (*data != 0);
	}

private:
	size_t		mMiddlePoint;
	int 		mLastHeight;
	int 		mPreviusHeight;
	size_t		mRowJumpSize;
	uchar		*mBaseAddr;
	int			mTopRow;
	int			mBottomRow;
};

#endif /* COLUMNANALYZER_H_ */
