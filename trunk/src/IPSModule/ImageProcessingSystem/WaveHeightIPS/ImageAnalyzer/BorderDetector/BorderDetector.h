/*
 * BorderDetector.h
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

#ifndef BORDERDETECTOR_H_
#define BORDERDETECTOR_H_


#include <opencv2/core/core.hpp>

#include "DebugUtil.h"
#include "ImageProcessor.h"

class BorderDetector : public ImageProcessor {
public:
	BorderDetector(const std::string &name = "BorderDetector");
	virtual ~BorderDetector();

	/* Sets the canny parametters to be used to detect the borders */
	void setThreshold1(double t) {mThreshold1 = t;}
	void setThreshold2(double t) {mThreshold2 = t;}
	void setL2Gradient(bool g) {mL2Gradient = g;}


	// Proccess the data on the CPU
	virtual errCode processData(cv::Mat &data) const;

	// Process the data on the GPU
	virtual errCode processData(cv::gpu::GpuMat &data) const;

private:
	// Canny parametters
	double				mThreshold1;
	double				mThreshold2;
	bool				mL2Gradient;


};

#endif /* BORDERDETECTOR_H_ */
