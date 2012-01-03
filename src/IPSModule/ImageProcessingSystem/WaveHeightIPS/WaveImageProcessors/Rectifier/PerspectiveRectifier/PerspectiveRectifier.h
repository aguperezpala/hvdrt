/*
 * PerspectiveRectifier.h
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

#ifndef PERSPECTIVERECTIFIER_H_
#define PERSPECTIVERECTIFIER_H_

#include <opencv2/core/core.hpp>

#include "IRectifier.h"

class PerspectiveRectifier : public IRectifier {
public:
	// Parameters
	enum {
		TLX_POINT_SRC,		// TOP LEFT point of source
		TLY_POINT_SRC,		// TOP LEFT point of source
		TRX_POINT_SRC,		// TOP RIGHT point of source
		TRY_POINT_SRC,		// TOP RIGHT point of source
		BLX_POINT_SRC,		// BOTTOM LEFT point of source
		BLY_POINT_SRC,		// BOTTOM LEFT point of source
		BRX_POINT_SRC,		// BOTTOM RIGHT point of source
		BRY_POINT_SRC,		// BOTTOM RIGHT point of source
		TLX_POINT_DST,		// TOP LEFT point of dest
		TLY_POINT_DST,		// TOP LEFT point of dest
		TRX_POINT_DST,		// TOP RIGHT point of dest
		TRY_POINT_DST,		// TOP RIGHT point of dest
		BLX_POINT_DST,		// BOTTOM LEFT point of dest
		BLY_POINT_DST,		// BOTTOM LEFT point of dest
		BRX_POINT_DST,		// BOTTOM RIGHT point of dest
		BRY_POINT_DST,		// BOTTOM RIGHT point of dest
		SET_POINTS,			// Set the configuration
		CLEAR_POINTS,		// clear the configuration
	};

public:
	PerspectiveRectifier();
	virtual ~PerspectiveRectifier();

	// Set a parameter to the ImageProcessor, used to configure the IP.
	errCode setParameter(int param, double value);

	// Gets a parameter value returned by param
	errCode getParameter(int param, double &value) const;

	// Proccess the data on the CPU
	errCode processData(cv::Mat &data) const;

	// Process the data on the GPU
	errCode processData(cv::gpu::GpuMat &data) const;

private:
	typedef std::vector<cv::Point2f>	PointVec;

	cv::Mat				mTransformMatrix;
	mutable cv::Mat		mDestMatrix;
	PointVec			mDestPoints;
	PointVec			mSrcPoints;
};

#endif /* PERSPECTIVERECTIFIER_H_ */
