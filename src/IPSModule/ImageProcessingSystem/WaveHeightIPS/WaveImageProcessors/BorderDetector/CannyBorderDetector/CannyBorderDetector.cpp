/*
 * CannyBorderDetector.cpp
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

#include "CannyBorderDetector.h"

CannyBorderDetector::CannyBorderDetector() :
BorderDetector("CannyBorderDetector"),
mThreshold1(10),
mThreshold2(80),
mL2Gradient(false)
{
	// TODO Auto-generated constructor stub

}

CannyBorderDetector::~CannyBorderDetector() {
	// TODO Auto-generated destructor stub
}

// Set a parameter to the ImageProcessor, used to configure the IP.
errCode CannyBorderDetector::setParameter(int param, double value)
{
	switch(param){
	case THRESHOLD_1:
		mThreshold1 = value;
		break;

	case THRESHOLD_2:
		mThreshold2 = value;
		break;

	case L2_GRADIENT:
		if(value != 0.0){
			mL2Gradient = true;
		} else {
			mL2Gradient = false;
		}
		break;

	default:
		return INVALID_PARAM;
	}

	return NO_ERROR;
}

// Gets a parameter value returned by param
errCode CannyBorderDetector::getParameter(int param, double &value) const
{
	switch(param){
	case THRESHOLD_1:
		value = mThreshold1;
		break;

	case THRESHOLD_2:
		value = mThreshold2;
		break;

	case L2_GRADIENT:
		if(mL2Gradient != 0.0){
			value = 1.0;
		} else {
			value = 0.0;
		}
		break;

	default:
		return INVALID_PARAM;
	}

	return NO_ERROR;
}

// Proccess the data on the CPU
errCode CannyBorderDetector::processData(cv::Mat &data) const
{
	// we will use the canny algorithm
	cv::cvtColor(data, data, CV_BGR2GRAY);
	cv::GaussianBlur(data, data, cv::Size(7,7), 1.5, 1.5);
	cv::Canny(data, data, mThreshold1, mThreshold2, 3, mL2Gradient);

	return NO_ERROR;
}

// Process the data on the GPU
errCode CannyBorderDetector::processData(cv::gpu::GpuMat &data) const
{
	return FEATURE_NOT_SUPPORTED;
}

