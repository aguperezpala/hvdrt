/*
 * MiddlePointClipping.cpp
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

#include "DebugUtil.h"
#include "MiddlePointClipping.h"

MiddlePointClipping::MiddlePointClipping() :
IClipping("MiddlePointClipping"),
mMiddlePoint(-1),
mNumColumns(-1)
{

}

MiddlePointClipping::~MiddlePointClipping()
{

}


// Set a parameter to the ImageProcessor, used to configure the IP.
errCode MiddlePointClipping::setParameter(int param, double value)
{
	switch(param){
	case MIDDLE_POINT_X:
		mMiddlePoint = value;
		break;

	case NUM_COLUMNS:
		mNumColumns = value;
		break;

	default:
		return INVALID_PARAM;
	}

	return NO_ERROR;
}

// Gets a parameter value returned by param
errCode MiddlePointClipping::getParameter(int param, double &value) const
{
	switch(param){
	case MIDDLE_POINT_X:
		value = mMiddlePoint;
		break;

	case NUM_COLUMNS:
		value = mNumColumns;
		break;

	default:
		return INVALID_PARAM;
	}

	return NO_ERROR;
}

// Proccess the data on the CPU
errCode MiddlePointClipping::processData(cv::Mat &data) const
{
	if(mAnalyzeMatrix.empty()){
		debug("Warning: Incomplete configuration, skipping\n");
		return NO_ERROR;
	}

	// else we copy the data to the new matrix
	ASSERT(mMiddlePoint > 0 && mMiddlePoint < data.cols);
	ASSERT(mNumColumns > 0 && (mMiddlePoint + (mNumColumns >> 1) ) < data.cols &&
			(mMiddlePoint - (mNumColumns >> 1)) > 0);

//	// copy the peace of the matrix
//	int deltaColumn = (mNumColumns >> 1);
//	int beginColumn = mMiddlePoint - deltaColumn;
//	int c = beginColumn;
//	int nc = mMiddlePoint + deltaColumn;
//	int l = 0;
//
//	uchar* srcData = data.data + c*data.elemSize();
//	uchar* destData = mAnalyzeMatrix.data;
//	// for all pixels
//	for (; l < data.rows; ++l) {
//		for (c = beginColumn; c < nc; ++c){
//			// copy the value to the analyzeMatrix
//			*destData = *srcData;
//			destData += mAnalyzeMatrix.elemSize();
//			srcData += data.elemSize();
//		}
//	}

	// TODO: Analyze if this function (colRange) really do what we want!
	int deltaColumn = (mNumColumns >> 1);
	int beginColumn = mMiddlePoint - deltaColumn;
	mAnalyzeMatrix = data.colRange(beginColumn, mMiddlePoint + deltaColumn);

	// replace the matrix to work with the new one
	data = mAnalyzeMatrix;

	return NO_ERROR;

}

// Process the data on the GPU
errCode MiddlePointClipping::processData(cv::gpu::GpuMat &data) const
{
	return FEATURE_NOT_SUPPORTED;
}
