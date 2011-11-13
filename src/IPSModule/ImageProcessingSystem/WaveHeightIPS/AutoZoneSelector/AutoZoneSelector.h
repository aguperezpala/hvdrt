/*
 * AutoZoneSelector.h
 *
 *  Created on: 11/11/2011
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

#ifndef AUTOZONESELECTOR_H_
#define AUTOZONESELECTOR_H_

#include <opencv2/core/core.hpp>

#include <list>

#include "DebugUtil.h"

class AutoZoneSelector {
public:
	typedef struct {
		int r;
		int g;
		int b;
	} RGB_t;
public:
	AutoZoneSelector();
	virtual ~AutoZoneSelector();

	/* Function used to set the the color in RGB to detect the lines */
	void setLinesColours(const RGB_t &color)
	{
		mLineColours = color;
	}

	/* Set the threshold of color to detect the lines */
	void setThresholdColor(int t)
	{
		mThresholdColor = t;
	}

	/* Try to find the points of the rectangle zone listing the points in the
	 * next oreder: TopLeft, TopRight, BottomLeft, BottomRight.
	 * Requires:
	 * 	@img			The image to be processed
	 * 	@points			The list where it will be putted the points
	 * Returns:
	 * 	true			on success
	 * 	false			otherwise
	 */
	bool getPoints(const cv::Mat &img, std::list<cv::Point> &points);

	/* Returns the image after been processed (if we want to see marked points) */
	const cv::Mat &getMarkedPoints(void)
	{
		return mImage;
	}

private:
	/* filter the image and let only the colors that we want */
	void colourFilter(cv::Mat &image, const RGB_t &color);


private:
	// Color of lines to be detected
	RGB_t					mLineColours;
	// Color lines threshold
	int						mThresholdColor;
	// The img where we will work
	cv::Mat					mImage;

};

#endif /* AUTOZONESELECTOR_H_ */
