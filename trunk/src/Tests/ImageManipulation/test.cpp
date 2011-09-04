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


#include <cv.h>
#include <highgui.h>
#include <iostream>


int main2(void)
{
	cv::Mat img;

	img = cv::imread("03.jpg");
	if(!img.data){
		std::cout << "Error reading the image\n";
		return 1;
	}

	cv::namedWindow("nonFlip");
	cv::imshow("nonFlip", img);



	//cv::flip(img,img,0);
	for(int i = 0; i < img.cols; ++i){
		img.at<cv::Vec3b>(i,i)[0] = 255;
		img.at<cv::Vec3b>(i,i)[1] = 0;
		img.at<cv::Vec3b>(i,i)[2] = 0;
	}

	cv::namedWindow("testw");
	cv::imshow("testw", img);

	cv::waitKey(0);


	return 0;
}
