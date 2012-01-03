/*
 * PerspectiveRectifier.cpp
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
#include "PerspectiveRectifier.h"

PerspectiveRectifier::PerspectiveRectifier() :
IRectifier("PerspectiveRectifier")
{
	mSrcPoints.resize(4);
	mDestPoints.resize(4);
}

PerspectiveRectifier::~PerspectiveRectifier()
{

}

// Set a parameter to the ImageProcessor, used to configure the IP.
errCode PerspectiveRectifier::setParameter(int param, double value)
{
	switch(param){
	case TLX_POINT_SRC:		// TOP LEFT point of source
		mSrcPoints[0].x = static_cast<float>(value);
		break;
	case TRX_POINT_SRC:		// TOP RIGHT point of source
		mSrcPoints[1].x = static_cast<float>(value);
		break;
	case BLX_POINT_SRC:		// BOTTOM LEFT point of source
		mSrcPoints[2].x = static_cast<float>(value);
		break;
	case BRX_POINT_SRC:		// BOTTOM RIGHT point of source
		mSrcPoints[3].x = static_cast<float>(value);
		break;
	case TLX_POINT_DST:		// TOP LEFT point of dest
		mDestPoints[0].x = static_cast<float>(value);
		break;
	case TRX_POINT_DST:		// TOP RIGHT point of dest
		mDestPoints[1].x = static_cast<float>(value);
		break;
	case BLX_POINT_DST:		// BOTTOM LEFT point of dest
		mDestPoints[2].x = static_cast<float>(value);
		break;
	case BRX_POINT_DST:		// BOTTOM RIGHT point of dest
		mDestPoints[3].x = static_cast<float>(value);
		break;
	case TLY_POINT_SRC:		// TOP LEFT point of source
		mSrcPoints[0].y = static_cast<float>(value);
		break;
	case TRY_POINT_SRC:		// TOP RIGHT point of source
		mSrcPoints[1].y = static_cast<float>(value);
		break;
	case BLY_POINT_SRC:		// BOTTOM LEFT point of source
		mSrcPoints[2].y = static_cast<float>(value);
		break;
	case BRY_POINT_SRC:		// BOTTOM RIGHT point of source
		mSrcPoints[3].y = static_cast<float>(value);
		break;
	case TLY_POINT_DST:		// TOP LEFT point of dest
		mDestPoints[0].y = static_cast<float>(value);
		break;
	case TRY_POINT_DST:		// TOP RIGHT point of dest
		mDestPoints[1].y = static_cast<float>(value);
		break;
	case BLY_POINT_DST:		// BOTTOM LEFT point of dest
		mDestPoints[2].y = static_cast<float>(value);
		break;
	case BRY_POINT_DST:		// BOTTOM RIGHT point of dest
		mDestPoints[3].y = static_cast<float>(value);
		break;
	case SET_POINTS:
	{
		if(mSrcPoints.size() != 4) {
			debug("Error: Number of source points isn't 4\n");
			return INCOMPLETE_CONFIGURATION;
		}
		if(mDestPoints.size() != 4) {
			debug("Error: Number of dest points isn't 4\n");
			return INCOMPLETE_CONFIGURATION;
		}
		cv::Point2f	source[4];
		cv::Point2f	dest[4];

		for(int i = mSrcPoints.size() - 1; i >= 0; --i){
			source[i] = mSrcPoints[i];
			dest[i] = mDestPoints[i];
		}

		// create the transformation matrix
		mTransformMatrix = cv::getPerspectiveTransform(source, dest);
	}

		break;
	case CLEAR_POINTS:
		mDestPoints.clear();
		mSrcPoints.clear();
		break;

	default:
		return INVALID_PARAM;
	}

	return NO_ERROR;
}

// Gets a parameter value returned by param
errCode PerspectiveRectifier::getParameter(int param, double &value) const
{
	switch(param){
	case TLX_POINT_SRC:		// TOP LEFT point of source
		value = static_cast<double>(mSrcPoints[0].x);
		break;
	case TRX_POINT_SRC:		// TOP RIGHT point of source
		value = static_cast<float>(mSrcPoints[1].x);
		break;
	case BLX_POINT_SRC:		// BOTTOM LEFT point of source
		value =  static_cast<float>(mSrcPoints[2].x);
		break;
	case BRX_POINT_SRC:		// BOTTOM RIGHT point of source
		value = static_cast<float>(mSrcPoints[3].x);
		break;
	case TLX_POINT_DST:		// TOP LEFT point of dest
		value = static_cast<float>(mDestPoints[0].x);
		break;
	case TRX_POINT_DST:		// TOP RIGHT point of dest
		value = static_cast<float>(mDestPoints[1].x);
		break;
	case BLX_POINT_DST:		// BOTTOM LEFT point of dest
		value = static_cast<float>(mDestPoints[2].x);
		break;
	case BRX_POINT_DST:		// BOTTOM RIGHT point of dest
		value = static_cast<float>(mDestPoints[3].x);
		break;
	case TLY_POINT_SRC:		// TOP LEFT point of source
		value = static_cast<double>(mSrcPoints[0].y);
		break;
	case TRY_POINT_SRC:		// TOP RIGHT point of source
		value = static_cast<float>(mSrcPoints[1].y);
		break;
	case BLY_POINT_SRC:		// BOTTOM LEFT point of source
		value =  static_cast<float>(mSrcPoints[2].y);
		break;
	case BRY_POINT_SRC:		// BOTTOM RIGHT point of source
		value = static_cast<float>(mSrcPoints[3].y);
		break;
	case TLY_POINT_DST:		// TOP LEFT point of dest
		value = static_cast<float>(mDestPoints[0].y);
		break;
	case TRY_POINT_DST:		// TOP RIGHT point of dest
		value = static_cast<float>(mDestPoints[1].y);
		break;
	case BLY_POINT_DST:		// BOTTOM LEFT point of dest
		value = static_cast<float>(mDestPoints[2].y);
		break;
	case BRY_POINT_DST:		// BOTTOM RIGHT point of dest
		value = static_cast<float>(mDestPoints[3].y);
		break;

	default:
		return INVALID_PARAM;
	}

	return NO_ERROR;
}

// Proccess the data on the CPU
errCode PerspectiveRectifier::processData(cv::Mat &data) const
{
	if(mTransformMatrix.empty()){
		debug("Error: No perspective matrix was set\n");
		return INCOMPLETE_CONFIGURATION;
	}

	// check if we have to create the dest matrix
	if(mDestMatrix.empty()){
		debug("Creating the Destination perspective transforamtion matrix\n");
		mDestMatrix = data.clone();
	}

	// apply the perspective transformation
	cv::warpPerspective(data,mDestMatrix,mTransformMatrix, data.size());
	data = mDestMatrix;

	return NO_ERROR;
}

// Process the data on the GPU
errCode PerspectiveRectifier::processData(cv::gpu::GpuMat &data) const
{
	return FEATURE_NOT_SUPPORTED;
}
