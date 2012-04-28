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
#include <sys/time.h>

static double getNowTimeStamp(void)
{
	struct timeval now;
	if(gettimeofday(&now, 0) < 0)
		return 0.0;

	return static_cast<double>(now.tv_sec + (
								static_cast<double>(now.tv_usec)/1000000.0));
}


static void processFrameCPU(cv::Mat &m)
{
	static cv::Mat aux;

	cv::cvtColor(m, aux, CV_BGR2GRAY);
	cv::GaussianBlur(aux ,m, cv::Size(7,7),1.5);
	cv::Canny(m, aux, 2, 15);
	m = aux;
}

static void processFrameGPU(cv::gpu::GpuMat &m)
{
	static cv::gpu::GpuMat aux;
	static cv::gpu::GpuMat aux2;
	static cv::gpu::GpuMat aux3;

	cv::gpu::cvtColor(m, aux, CV_BGR2GRAY);
	cv::gpu::GaussianBlur(aux,aux2, cv::Size(7,7),1.5);
	cv::gpu::Canny(aux2, aux3, 2, 15);
	aux3.copyTo(m);
}


static void testVelocityOneImage(cv::Mat &m)
{
	cv::Mat cpy,aux;
	m.copyTo(cpy);
	int totalIt = 200;

	double accum = 0;
	double cpu1 = getNowTimeStamp();
	for(int i = 0; i < totalIt; ++i){
		double cput1 = getNowTimeStamp();
		m.copyTo(cpy);
		cv::cvtColor(cpy, aux, CV_BGR2GRAY);
		cv::GaussianBlur(aux ,cpy, cv::Size(7,7),1.5);
		cv::Canny(cpy, aux, 2, 15);
		double cput2 = getNowTimeStamp();
		accum += cput2 - cput1;
	}
	double cpu2 = getNowTimeStamp();
	std::cout << "Time CPU: " << cpu2 - cpu1 << "\t Prom: " << accum / totalIt << std::endl;

	cv::gpu::GpuMat gcpy, gaux, aux2, aux3, aux4;
	gcpy.upload(m);
	gaux.upload(m);
//	aux2.upload(m);
//	aux3.upload(m);
	aux4.upload(m);

	accum = 0;

	double gpu1 = getNowTimeStamp();

	for(int i = 0; i < totalIt; ++i){
		double gput1 = getNowTimeStamp();
		aux4.copyTo(gcpy);
		cv::gpu::cvtColor(gcpy, gaux, CV_BGR2GRAY);
		cv::gpu::GaussianBlur(gaux,gcpy, cv::Size(7,7),1.5);
		cv::gpu::Canny(gcpy, gaux, 2, 15);
		double gput2 = getNowTimeStamp();
		accum += gput2 - gput1;
	}
	double gpu2 = getNowTimeStamp();
	std::cout << "Time GPU: " << gpu2 - gpu1 << "\t Prom: " << accum / totalIt << std::endl;

}

int main(int argc, char **args)
{
	if(argc < 2){
		std::cout << "Error, falta nombre archivo\n";
		return -1;
	}

	cv::VideoCapture vc(args[1]);

	if(!vc.isOpened()){
		std::cout << "Error: Img " << args[1] << "not exists\n";
		return -1;
	}

	double th1 = 2, th2 = 20;


//	cv::Mat frame;
//	double cpu1 = getNowTimeStamp();
//
//	while(vc.isOpened()){
//		if(!vc.read(frame)){
//			break;
//		}
//
//		// else process the frame in the cpu
//		processFrameCPU(frame);
//	}
//
//	double cpu2 = getNowTimeStamp();
//
//	std::cout << "Time of video processing on CPU was: " << cpu2 - cpu1 << std::endl;

	cv::Mat frame2;
	cv::gpu::GpuMat gpm, aux, aux1, aux2, aux3;

	vc.release();
	vc.open(args[1]);

	if(!vc.isOpened()){
		std::cout << "Error: Img " << args[1] << "not exists\n";
		return -1;
	}


	cv::Mat img;
	vc.read(img);
	testVelocityOneImage(img);

	return 0;

	double gpu1 = getNowTimeStamp();

	while(vc.isOpened()){
		if(!vc.read(frame2)){
			break;
		}

		// else process the frame in the cpu
		gpm = frame2;
		cv::gpu::cvtColor(gpm, aux, CV_BGR2GRAY);
		cv::gpu::GaussianBlur(aux,aux2, cv::Size(7,7),1.5);
		cv::gpu::Canny(aux2, aux3, 2, 15);
		aux3.copyTo(gpm);
//		processFrameGPU(gpm);
	}

	double gpu2 = getNowTimeStamp();
	std::cout << "Time of video processing on GPU was: " << gpu2 - gpu1 << std::endl;

	cv::waitKey();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
