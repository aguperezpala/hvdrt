/*
 * AutoZoneSelector.cpp
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

#include <opencv2/imgproc/imgproc.hpp>

#include "AutoZoneSelector.h"


void AutoZoneSelector::colourFilter(cv::Mat &image, const RGB_t &color)
{
	cv::Scalar min = CV_RGB(color.r - mThresholdColor/2,
			color.g - mThresholdColor/2,
			color.b - mThresholdColor/2);
	cv::Scalar max = CV_RGB(color.r + mThresholdColor/2,
				color.g + mThresholdColor/2,
				color.b + mThresholdColor/2);

	cv::inRange(image, min, max, image);

}



AutoZoneSelector::AutoZoneSelector() :
mThresholdColor(60)
{

}

AutoZoneSelector::~AutoZoneSelector() {
	// TODO Auto-generated destructor stub
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
bool AutoZoneSelector::getPoints(const cv::Mat &img, std::list<cv::Point> &points)
{
	if(img.empty()){
		debug("Error: empty image\n");
		return false;
	}

	// clone the image
	mImage = img.clone();

	// apply the color filter
	colourFilter(mImage, mLineColours);




}
