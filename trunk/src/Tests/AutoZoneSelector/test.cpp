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


#define BOUNDARY_COLOR	20

using namespace cv;

typedef struct {
	int r;
	int g;
	int b;
} RGB_t;

static void colourFilter(cv::Mat &image, const RGB_t &color)
{
	cv::Scalar min = CV_RGB(color.r - BOUNDARY_COLOR/2,
			color.g - BOUNDARY_COLOR/2,
			color.b - BOUNDARY_COLOR/2);
	cv::Scalar max = CV_RGB(color.r + BOUNDARY_COLOR/2,
				color.g + BOUNDARY_COLOR/2,
				color.b + BOUNDARY_COLOR/2);

	cv::inRange(image, min, max, image);


}


static void detectZone(cv::Mat &mat, const RGB_t &colour)
{
	if(mat.empty()){
		debug("Error: Empty img\n");
		return;
	}

	cv::Mat final = mat.clone();
	//
	// first apply the filter
//	colourFilter(mat, colour);


	// now try to find the four points
	cv::cvtColor(mat,mat,CV_BGR2GRAY);
	cv::GaussianBlur(mat, mat, cv::Size(7,7), 1.5, 1.5);
	cv::Canny(mat, mat, 10, 50, 3);
	cv::namedWindow("Transformed Image1");
	cv::imshow("Transformed Image1", mat);

	std::vector<Point2f> corners;
	cv::goodFeaturesToTrack(mat, corners, 4, 0.01, 100);
	std::cout << "corners detected: " << corners.size() << std::endl;
	for(int i = corners.size()-1; i >= 0; --i){
		line( final, Point(corners[i].x, corners[i].y),
					Point(corners[i].x, corners[i].y), Scalar(0,0,255), 3, 8 );
	}

//	std::vector<cv::Vec4i> lines;
//	HoughLinesP( mat, lines, 1, CV_PI/180, 80, 30, 10 );
//	std::cout << "Lines detected: " << lines.size() << std::endl;
//	for( size_t i = 0; i < lines.size(); i++ )
//	{
//		line( final, Point(lines[i][0], lines[i][1]),
//			Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
//	}
//	HoughLines( mat, lines, 1, CV_PI/180, 100 );
//
//	for( size_t i = 0; i < lines.size(); i++ )
//	{
//		float rho = lines[i][0];
//		float theta = lines[i][1];
//		double a = cos(theta), b = sin(theta);
//		double x0 = a*rho, y0 = b*rho;
//		Point pt1(cvRound(x0 + 1000*(-b)),
//				  cvRound(y0 + 1000*(a)));
//		Point pt2(cvRound(x0 - 1000*(-b)),
//				  cvRound(y0 - 1000*(a)));
//		line( final, pt1, pt2, Scalar(0,0,255), 3, 8 );
//	}

	cv::namedWindow("Transformed Image");
	cv::imshow("Transformed Image", final);

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

	RGB_t rgb = {136,130,131};

	detectZone(img,rgb);

	waitKey();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
