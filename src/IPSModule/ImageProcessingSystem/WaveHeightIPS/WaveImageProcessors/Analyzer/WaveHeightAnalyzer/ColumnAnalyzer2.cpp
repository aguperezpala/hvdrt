/*
 * ColumnAnalyzer.cpp
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

#include "ColumnAnalyzer.h"
ColumnAnalyzer::ColumnAnalyzer() :
mMiddlePoint(0),
mLastHeight(0),
mPreviusHeight(0),
mRowJumpSize(0),
mBaseAddr(0),
mTopRow(0),
mBottomRow(0)
{
	// TODO Auto-generated constructor stub

}

ColumnAnalyzer::~ColumnAnalyzer()
{
	// TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////////////////////
void ColumnAnalyzer::setMiddlePoint(size_t middle)
{
	mMiddlePoint = middle;
	if(mLastHeight == 0){
		mLastHeight = static_cast<int>(mMiddlePoint);
	}
}

////////////////////////////////////////////////////////////////////////////////
void ColumnAnalyzer::configure(cv::Mat &m, size_t numColumn, size_t topRow,
		size_t bottomRow)
{
	// we will extract the info needed to analyze the numColumn-th column
	ASSERT(static_cast<int>(numColumn) <= m.cols);
	ASSERT(topRow > mBottomRow);

	mBaseAddr = m.data + mBottomRow*m.step + numColumn*m.elemSize();
	mRowJumpSize = m.step;
	mTopRow = topRow;
	mBottomRow = bottomRow;
	updateSize();
}

////////////////////////////////////////////////////////////////////////////////
void ColumnAnalyzer::configure(size_t topRow, size_t bottomRow,
		uchar *baseAddr, size_t jumpSize)
{
	ASSERT(topRow > mBottomRow);
	mTopRow = topRow;
	mBottomRow = bottomRow;
	mBaseAddr = baseAddr;
	mRowJumpSize = jumpSize;
	updateSize();
}

////////////////////////////////////////////////////////////////////////////////
void ColumnAnalyzer::analyze(void)
{
	ASSERT(mLastHeight < mTopRow && mLastHeight >= mBottomRow);

	// store the last value
	mPreviusHeight = mLastHeight;

	// we will first start analyzing in the lastHeight
	if(isPixelSetUsingLastSnapshoot(mLastHeight)){
		// do nothing, the heigh of the wave is the same
		updateLastSnapshoot();
		return;
	}

	// the height is not the same, start to check now
	// REMEMBER that the position in the column are inverted (0 row is the
	// upper row and NumRows row is the bottom row in the image)
	int upperPos = mLastHeight + 1;
	int lowerPos = mLastHeight - 1;

	while(upperPos < mTopRow || lowerPos >= mBottomRow){
		// analyze by parts
		if(upperPos < mTopRow){
			if(isPixelSetUsingLastSnapshoot(upperPos)){
				// finish here? by now yes
				mLastHeight = upperPos;
				break;
			}
			++upperPos;
		}

		// analyze now the lower
		if(lowerPos >= mBottomRow){
			if(isPixelSetUsingLastSnapshoot(lowerPos)){
				// finish now
				mLastHeight = lowerPos;
				break;
			}
			--lowerPos;
		}

	}
	updateLastSnapshoot();

}


////////////////////////////////////////////////////////////////////////////////
void ColumnAnalyzer::correctRelativeHeight(int h)
{
	mLastHeight = mMiddlePoint - h;
	ASSERT(mLastHeight < mTopRow && mLastHeight >= mBottomRow);
}





