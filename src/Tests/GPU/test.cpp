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
#include <sstream>



int main(int argc, char **args)
{
	if(argc < 2){
		std::cout << "Error, falta nombre archivo\n";
		return -1;
	}

	cv::Mat img = cv::imread(args[1]);

	if(img.empty()){
		std::cout << "Error: Img " << args[1] << "not exists\n";
		return -1;
	}

	double th1 = 2, th2 = 20;
	// check canny param
	if(argc == 4){
		std::stringstream s1;
		s1 << args[2];
		s1 >> th1;
		std::stringstream s2;
		s2 << args[3];
		s2 >> th2;

		std::cout << "Usando parametros: " << th1 << "\t" << th2 << std::endl;
	}

	cv::namedWindow("Real Image");
//	cv::namedWindow("Canny Image");

	// port to gpu
	cv::gpu::GpuMat gm, gm2, gm3, gm4;
//	cv::cvtColor(img,img,CV_BGR2GRAY);
//	gm = img;

	gm.upload(img);
//	gm.convertTo(gm2, CV_MAKE_TYPE(CV_32SC1, gm.channels()));

	cv::gpu::cvtColor(gm, gm2, CV_BGR2GRAY);
	cv::gpu::GaussianBlur(gm2,gm, cv::Size(7,7),1.5);
//	cv::gpu::cvtColor(gm, gm, CV_BGR2GRAY);
	cv::gpu::Canny(gm, gm2, th1, th2);
//	cv::gpu::add(gm2, gm2, gm2);

	// download again to the memory
//	gm.download(img);
	cv::Mat auxMat;
//	gm.download(auxMat);
	auxMat = gm2;
	cv::imshow("Real Image", auxMat);


//	cv::cvtColor(img, img, CV_BGR2GRAY);
//	cv::Canny(img, img, th1, th2);
//
//	cv::imshow("Canny Image", img);
	cv::waitKey();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
