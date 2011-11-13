/*
 * PerspectiveTransformator.h
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

#ifndef PERSPECTIVETRANSFORMATOR_H_
#define PERSPECTIVETRANSFORMATOR_H_


#include <opencv2/core/core.hpp>

#include "DebugUtil.h"
#include "ImageProcessor.h"

class PerspectiveTransformator : public ImageProcessor {
public:
	PerspectiveTransformator(const std::string &name = "PerspectiveTransformator");
	virtual ~PerspectiveTransformator();

	/* Set the list of points used to get the transformation matrix.
	 * Params:
	 * 	@sourcePoints		The points in the real image (source image)
	 * 	@destPoints			The points where it will be mapped the source points
	 * 						into the dest image.
	 * Requires:
	 * 	sourcePoints.size() == destPoints.size() == 4
	 * Note: the dest points will set the size of the destination image and the
	 * 		 i-th source point will be mapped to the i-th dest point
	 *
	 * Returns:
	 * 	true				On success
	 *	false				otherwise
	 */
	bool setTransformationPoints(const std::vector<cv::Point2f> &sourcePoints,
			const std::vector<cv::Point2f> &destPoints);

	// Proccess the data on the CPU
	virtual errCode processData(cv::Mat &data) const;

	// Process the data on the GPU
	virtual errCode processData(cv::gpu::GpuMat &data) const ;

private:
	cv::Mat				mTransformMatrix;
	mutable cv::Mat		mDestMatrix;
};

#endif /* PERSPECTIVETRANSFORMATOR_H_ */
