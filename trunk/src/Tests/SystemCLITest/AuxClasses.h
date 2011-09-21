/*
 * AuxClasses.h
 *
 *  Created on: 16/09/2011
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

#ifndef AUXCLASSES_H_
#define AUXCLASSES_H_

#include <iostream>
#include <string>

#include "DebugUtil.h"
#include "ImageGenerator.h"
#include "ImageAnalyzer.h"



class TestImageProcessor : public ImageProcessor {
public:
	TestImageProcessor() : ImageProcessor("TestImageProcessor") {}
	 ~TestImageProcessor(){};

	// Proccess the data on the CPU
	 errCode processData(cv::Mat &data) const
	{
		cvtColor(data, data, CV_BGR2GRAY);
		GaussianBlur(data, data, cv::Size(7,7), 1.5, 1.5);
		cv::Canny(data, data, 10, 50, 3);
		 return NO_ERROR;
	}

	// Process the data on the GPU
	 errCode processData(cv::gpu::GpuMat &data) const
	{
		 cv::gpu::cvtColor(data,data,CV_BGR2GRAY);
		 cv::gpu::GaussianBlur(data, data, cv::Size(7,7), 1.5, 1.5);
		 cv::gpu::Canny(data, data, 10, 50, 3);
		return FEATURE_NOT_SUPPORTED;
	}

};

class TestImageProcessor1 : public ImageProcessor {
public:
	TestImageProcessor1() : ImageProcessor("TestImageProcessor1") {}
	virtual ~TestImageProcessor1(){};

	// Proccess the data on the CPU
	virtual errCode processData(cv::Mat &data) const{cvtColor(data, data, CV_BGR2GRAY); return NO_ERROR;	}

	// Process the data on the GPU
	virtual errCode processData(cv::gpu::GpuMat &data) const{return FEATURE_NOT_SUPPORTED;}

};
class TestImageProcessor2 : public ImageProcessor {
public:
	TestImageProcessor2() : ImageProcessor("TestImageProcessor2") {}
	virtual ~TestImageProcessor2(){};

	// Proccess the data on the CPU
	virtual errCode processData(cv::Mat &data) const{GaussianBlur(data, data, cv::Size(7,7), 1.5, 1.5);	 return NO_ERROR;}

	// Process the data on the GPU
	virtual errCode processData(cv::gpu::GpuMat &data) const{return FEATURE_NOT_SUPPORTED;}

};
class TestImageProcessor3 : public ImageProcessor {
public:
	TestImageProcessor3() : ImageProcessor("TestImageProcessor3") {}
	virtual ~TestImageProcessor3(){};

	// Proccess the data on the CPU
	virtual errCode processData(cv::Mat &data) const{cv::Canny(data, data, 10, 50, 3);	 return NO_ERROR;}

	// Process the data on the GPU
	virtual errCode processData(cv::gpu::GpuMat &data) const{return FEATURE_NOT_SUPPORTED;}

};



class TestFrameListener1 : public FrameListener{
public:
	TestFrameListener1()
	{
		ia.addNewProcessor(&t1);

	}
	virtual ~TestFrameListener1(){};

	/* Process a frame (blocking function) */
	void processFrame(Frame &frame){
		std::cout << "TestFrameListener1: processFrame: " << ia.processImageOnCPU(frame, true) << std::endl;
		cv::imshow("TestFrameListener1", frame.data);
	}

private:
	ImageAnalyzer ia;
	TestImageProcessor t1;
};

class TestFrameListener2 : public FrameListener{
public:
	TestFrameListener2()
	{
		ia.addNewProcessor(&t1);
		ia.addNewProcessor(&t2);
		ia.addNewProcessor(&t3);
	}
	virtual ~TestFrameListener2(){};

	/* Process a frame (blocking function) */
	void processFrame(Frame &frame){
		std::cout << "TestFrameListener2: processFrame: " << ia.processImageOnCPU(frame, true) << std::endl;
		cv::imshow("TestFrameListener2",frame.data);
	}

private:
	ImageAnalyzer ia;
	TestImageProcessor1 t1;
	TestImageProcessor2 t2;
	TestImageProcessor3 t3;
};


#endif /* AUXCLASSES_H_ */
