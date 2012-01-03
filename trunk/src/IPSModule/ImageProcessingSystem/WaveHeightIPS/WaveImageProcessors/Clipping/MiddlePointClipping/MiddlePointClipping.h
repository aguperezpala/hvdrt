/*
 * MiddlePointClipping.h
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

#ifndef MIDDLEPOINTCLIPPING_H_
#define MIDDLEPOINTCLIPPING_H_

#include <opencv2/core/core.hpp>

#include "IClipping.h"

class MiddlePointClipping : public IClipping {
public:
	enum {
		MIDDLE_POINT_X,		// where is the middle point in x
		NUM_COLUMNS,		// how many columns we want to analyze
	};
public:
	MiddlePointClipping();
	virtual ~MiddlePointClipping();

	// Set a parameter to the ImageProcessor, used to configure the IP.
	errCode setParameter(int param, double value);

	// Gets a parameter value returned by param
	errCode getParameter(int param, double &value) const;

	// Proccess the data on the CPU
	errCode processData(cv::Mat &data) const;

	// Process the data on the GPU
	errCode processData(cv::gpu::GpuMat &data) const;

private:
	// The matrix where we will work
	mutable cv::Mat		mAnalyzeMatrix;
	int					mMiddlePoint;
	int					mNumColumns;
};

#endif /* MIDDLEPOINTCLIPPING_H_ */
