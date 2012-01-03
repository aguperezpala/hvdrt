/*
 * WaveHeightIPFactory.h
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

#ifndef WAVEHEIGHTIPFACTORY_H_
#define WAVEHEIGHTIPFACTORY_H_

#include "ImageProcessor.h"


class WaveHeightIPFactory {
public:
	enum {
		IP_RECTIFIER,		// The IP used to rectify the image
		IP_CLIPPING,		// The IP used to clip the image
		IP_BORDER_DETECTOR,	// Canny
		IP_WH_ANALYZER,		// The wave height (pixel) analyzer
		// IP_EXTRA
	};

public:
	// Creates a ImageProcessor by param.
	// In case that the processor to be created is not supported, 0 is returned
	ImageProcessor *getImageProcessor(int);
};

#endif /* WAVEHEIGHTIPFACTORY_H_ */
