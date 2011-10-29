/*
 * CoordsInterpreter.h
 *
 *  Created on: 23/10/2011
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

#ifndef COORDSINTERPRETER_H_
#define COORDSINTERPRETER_H_


#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "ImageProcessor.h"



class CoordsInterpreter : public ImageProcessor {
public:
	// This data will be used to store the points of the image that
	// are painted (assuming that we are working with a black/white image
	// where the white image are the points that we want to analyze
	typedef std::vector<std::vector<int> >	Data;


public:
	CoordsInterpreter();
	virtual ~CoordsInterpreter();

	/* Sets the rectangle where it will be analyzed the image. */
	void setAnalyzeZone(const cv::Rect &zone);
	const cv::Rect &getAnalyzeZone(void) const {return mAnalyzeZone;}

	/* Sets the Data where it will be stored the results.
	 * REQUIRES:
	 * 		data		!= 0
	 * 		data.size	== zone.width
	 */
	void setData(Data *data);

	/* This class will retrieve all the points painted ("white points from a
	 * black image), and it will store that points coordinates (from bottom to
	 * top) in the Data structure. This means that the points painted in the
	 * bottom of the zone will have the lower values in the Data structure, and
	 * the points that are painted in the top of the zone will have the high
	 * values
	 * In each frame the Data will be cleared, so we have to retrieve the info
	 * in every frame
	 */

	// Process the data on the CPU
	errCode processData(cv::Mat &data) const;

	// Process the data on the GPU
	errCode processData(cv::gpu::GpuMat &data) const;

private:
	/* function used to clear the data */
	void clearData(void) const;

private:
	// The zone to be analyzed
	cv::Rect				mAnalyzeZone;
	Data					*mData;

};

#endif /* COORDSINTERPRETER_H_ */
