/*
 * CannyBorderDetector.h
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

#ifndef CANNYBORDERDETECTOR_H_
#define CANNYBORDERDETECTOR_H_



#include "IBorderDetector.h"

class CannyBorderDetector : public IBorderDetector {
public:
	enum {
		THRESHOLD_1,
		THRESHOLD_2,
		L2_GRADIENT,
	};

public:
	CannyBorderDetector();
	virtual ~CannyBorderDetector();

	// Set a parameter to the ImageProcessor, used to configure the IP.
	errCode setParameter(int param, double value);

	// Gets a parameter value returned by param
	errCode getParameter(int param, double &value) const;

	// Proccess the data on the CPU
	errCode processData(cv::Mat &data) const;

	// Process the data on the GPU
	errCode processData(cv::gpu::GpuMat &data) const;

private:
	double			mThreshold1;
	double			mThreshold2;
	bool			mL2Gradient;

};

#endif /* CANNYBORDERDETECTOR_H_ */
