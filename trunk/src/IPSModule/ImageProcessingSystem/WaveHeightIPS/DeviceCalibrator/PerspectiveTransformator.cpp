/*
 * PerspectiveTransformator.cpp
 *
 *  Created on: 12/11/2011
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

#include "PerspectiveTransformator.h"

PerspectiveTransformator::PerspectiveTransformator(const std::string &name) :
ImageProcessor(name)
{
	// TODO Auto-generated constructor stub

}

PerspectiveTransformator::~PerspectiveTransformator() {
	// TODO Auto-generated destructor stub
}



bool PerspectiveTransformator::setTransformationPoints(
		const std::vector<cv::Point2f> &sourcePoints,
		const std::vector<cv::Point2f> &destPoints)
{
	if(sourcePoints.size() != 4) {
		debug("Error: Number of source points isn't 4\n");
		return false;
	}
	if(destPoints.size() != 4) {
		debug("Error: Number of dest points isn't 4\n");
		return false;
	}
	cv::Point2f	source[4];
	cv::Point2f	dest[4];

	for(int i = sourcePoints.size() - 1; i >= 0; --i){
		source[i] = sourcePoints[i];
		dest[i] = destPoints[i];
	}

	// create the transformation matrix
	mTransformMatrix = cv::getPerspectiveTransform(source, dest);

	return true;
}

// Proccess the data on the CPU
errCode PerspectiveTransformator::processData(cv::Mat &data) const
{
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
errCode PerspectiveTransformator::processData(cv::gpu::GpuMat &data) const
{
	return FEATURE_NOT_SUPPORTED;
}
