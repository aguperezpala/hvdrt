/*
 * test.cpp
 *
 *  Created on: 02/09/2011
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
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>

#include "DebugUtil.h"
#include "CoordsInterpreter.h"


using namespace cv;


static void detectZone(cv::Mat &mat)
{

}


int main(int, char**)
{
	cv::Mat img = cv::imread("img.jpg");

	if(img.empty()){
		debug("Error: Img img.jpg not exists\n");
		return -1;
	}

	cv::namedWindow("Real Image");
	cv::imshow("Real Image", img);

	detectZone(img);

	waitKey();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
