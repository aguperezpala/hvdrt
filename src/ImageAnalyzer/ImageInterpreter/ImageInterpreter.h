/*
 * ImageInterpreter.h
 *
 *  Created on: 13/09/2011
 *      Author: agustin
 */

#ifndef IMAGEINTERPRETER_H_
#define IMAGEINTERPRETER_H_


#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>



class ImageInterpreter {
public:
	ImageInterpreter();
	virtual ~ImageInterpreter(){};

	// Proccess the data on the CPU
	virtual int processData(cv::Mat &data) = 0;

	// Process the data on the GPU
	virtual int processData(cv::gpu::GpuMat &data) = 0;

};


#endif /* IMAGEINTERPRETER_H_ */
