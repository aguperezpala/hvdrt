/*
 * test.cpp
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


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include "DebugUtil.h"
#include "MiddlePointClipping.h"
#include "PerspectiveRectifier.h"
#include "CannyBorderDetector.h"
#include "WaveHeightAnalyzer.h"


#define IMG_FNAME	"/home/agustin/facu/tesis/muestreo_videos_lab/Pantallazo9.png"



static void testMiddlePointClipping(void)
{
	MiddlePointClipping mpc;

	cv::Mat img = cv::imread(IMG_FNAME);
	ASSERT(!img.empty());

	cv::namedWindow("testMiddlePointClipping_REAL");
	cv::imshow("testMiddlePointClipping_REAL", img);

	ASSERT(mpc.setParameter(MiddlePointClipping::MIDDLE_POINT_X, 848.0) == NO_ERROR);
	ASSERT(mpc.setParameter(MiddlePointClipping::NUM_COLUMNS, 50) == NO_ERROR);

	// process the image and see the result
	ASSERT(mpc.processData(img) == NO_ERROR);
	cv::namedWindow("testMiddlePointClipping_PROCESSED");
	cv::imshow("testMiddlePointClipping_PROCESSED", img);


	cv::waitKey();

}

static void testPerspectiveRectifier(void)
{
	PerspectiveRectifier pr;

	cv::Mat img = cv::imread(IMG_FNAME);
	ASSERT(!img.empty());

	cv::namedWindow("testPerspectiveRectifier_REAL");
	cv::imshow("testPerspectiveRectifier_REAL", img);


	ASSERT(pr.setParameter(PerspectiveRectifier::TLX_POINT_SRC, 614) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::TLY_POINT_SRC, 477) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::TRX_POINT_SRC, 992) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::TRY_POINT_SRC, 479) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::BLX_POINT_SRC, 576) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::BLY_POINT_SRC, 920) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::BRX_POINT_SRC, 1011) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::BRY_POINT_SRC, 906) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::TLX_POINT_DST, 0.0) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::TLY_POINT_DST, 0.0) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::TRX_POINT_DST, img.cols) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::TRY_POINT_DST, 0) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::BLX_POINT_DST, 0) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::BLY_POINT_DST, img.rows) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::BRX_POINT_DST, img.cols) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::BRY_POINT_DST, img.rows) == NO_ERROR);
	ASSERT(pr.setParameter(PerspectiveRectifier::SET_POINTS, 0) == NO_ERROR);

	// process the image and see the result
	ASSERT(pr.processData(img) == NO_ERROR);
	cv::namedWindow("testPerspectiveRectifier_PROCESSED");
	cv::imshow("testPerspectiveRectifier_PROCESSED", img);


	cv::waitKey();
}

static void testCannyBorderDetector(void)
{
	CannyBorderDetector cbd;

	cv::Mat img = cv::imread(IMG_FNAME);
	ASSERT(!img.empty());

	cv::namedWindow("testCannyBorderDetector_REAL");
	cv::imshow("testCannyBorderDetector_REAL", img);


	ASSERT(cbd.setParameter(CannyBorderDetector::THRESHOLD_1, 2) == NO_ERROR);
	ASSERT(cbd.setParameter(CannyBorderDetector::THRESHOLD_2, 20) == NO_ERROR);
	ASSERT(cbd.setParameter(CannyBorderDetector::L2_GRADIENT, 0) == NO_ERROR);


	// process the image and see the result
	ASSERT(cbd.processData(img) == NO_ERROR);
	cv::namedWindow("testCannyBorderDetector_PROCESSED");
	cv::imshow("testCannyBorderDetector_PROCESSED", img);


	cv::waitKey();
}

static void testWaveHeightAnalyzer(void)
{
	WaveHeightAnalyzer cbd;

	cv::Mat img = cv::imread(IMG_FNAME);
	ASSERT(!img.empty());

	cv::namedWindow("testWaveHeightAnalyzer_REAL");
	cv::imshow("testWaveHeightAnalyzer_REAL", img);

	// TODO: complete this function (we have first to apply canny before use the
	// processData() function)

	// process the image and see the result
	ASSERT(cbd.processData(img) == NO_ERROR);
	cv::namedWindow("testWaveHeightAnalyzer_PROCESSED");
	cv::imshow("testWaveHeightAnalyzer_PROCESSED", img);


	cv::waitKey();
}

int main(void)
{
//	testMiddlePointClipping();
//	testPerspectiveRectifier();
//	testCannyBorderDetector();
	testWaveHeightAnalyzer();

	return 0;
}
