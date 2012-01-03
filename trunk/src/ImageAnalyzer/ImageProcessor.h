/*
 * ImageProcessor.h
 *
 *  Created on: 16/09/2011
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

#ifndef IMAGEPROCESSOR_H_
#define IMAGEPROCESSOR_H_


#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>

#include "GlobalDefines.h"


class ImageProcessor {
public:
	ImageProcessor(const std::string &name = "Unnamed") :
		mName(name)
	{};

	virtual ~ImageProcessor(){};

	// returns the name of the ImageProcessor
	const std::string &getName(void) const {return mName;}

	// Set a parameter to the ImageProcessor, used to configure the IP.
	virtual errCode setParameter(int param, double value) {return FEATURE_NOT_SUPPORTED;}

	// Gets a parameter value returned by param
	virtual errCode getParameter(int param, double &value) const {return FEATURE_NOT_SUPPORTED;}

	// Proccess the data on the CPU
	virtual errCode processData(cv::Mat &data) const = 0;

	// Process the data on the GPU
	virtual errCode processData(cv::gpu::GpuMat &data) const = 0;

private:
	std::string		mName;

};

#endif /* IMAGEPROCESSOR_H_ */
