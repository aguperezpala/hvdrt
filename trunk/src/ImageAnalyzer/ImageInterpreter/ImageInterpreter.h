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


#include "ImageProcessor.h"


class ImageInterpreter : public ImageProcessor {
public:
	ImageInterpreter();
	virtual ~ImageInterpreter(){};

	// Proccess the data on the CPU
	virtual errCode processData(cv::Mat &data) const = 0;

	// Process the data on the GPU
	virtual errCode processData(cv::gpu::GpuMat &data) const  = 0;

};


#endif /* IMAGEINTERPRETER_H_ */
