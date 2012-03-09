/*
 * ImageAnalyzer.cpp
 *
 *  Created on: 13/09/2011
 *      Author: agustin
 */
#include "ImageAnalyzer.h"



////////////////////////////////////////////////////////////////////////////////
ImageAnalyzer::ImageAnalyzer()
{
	// TODO Auto-generated constructor stub

}

////////////////////////////////////////////////////////////////////////////////
ImageAnalyzer::~ImageAnalyzer()
{
	// TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////////////////////
/* Add a new image processor to the queue. The ImageProcessor will be
 * executed in the order that they will be put here
 */
void ImageAnalyzer::addNewProcessor(const ImageProcessor *ip)
{
	if(std::find(mImageProcessors.begin(), mImageProcessors.end(), ip) !=
			mImageProcessors.end()){
		debug("Trying to add a ImageProccesor that already exists\n");
		return;
	}
	mImageProcessors.push_back(ip);
}

////////////////////////////////////////////////////////////////////////////////
void ImageAnalyzer::addNewProcessors(const std::list<const ImageProcessor*> &l)
{
	removeAllProcessors();
	mImageProcessors = l;
}
////////////////////////////////////////////////////////////////////////////////
void ImageAnalyzer::removeProcessor(const ImageProcessor *ip)
{
	std::list<const ImageProcessor*>::iterator it = std::find(mImageProcessors.begin(),
			mImageProcessors.end(), ip);

	if(it == mImageProcessors.end()){
		debug("Trying to remove a ImageProccesor that not exists\n");
		return;
	}

	mImageProcessors.erase(it);
}

////////////////////////////////////////////////////////////////////////////////
void ImageAnalyzer::removeAllProcessors(void)
{
	mImageProcessors.clear();
}

////////////////////////////////////////////////////////////////////////////////
/* Execute the ImageAnalyzer using the CPU.
 * Params:
 * 	@frame			the frame to be processed
 * 	@trackProcess	if we want to add timeStamps to the frame.timestamp
 * Returns the errCode
 */
errCode ImageAnalyzer::processImageOnCPU(Frame &frame, bool trackProcess)
{
	std::list<const ImageProcessor*>::iterator it = mImageProcessors.begin();
	errCode err = NO_ERROR;

	debug("Start to process frame on CPU\n");

	for(; it != mImageProcessors.end(); ++it){
		const ImageProcessor *imgProc = *it;

		// some log :)
		debug("Processing frame with: %s\n", imgProc->getName().c_str());

		if(trackProcess){
			frame.timestamp.addTimestamp("Processing with " + imgProc->getName());
		}

		err = imgProc->processData(frame.data);
		if(err != NO_ERROR){
			debug("Some error occurred during the processing of the data %d\t%s\n",
					err, imgProc->getName().c_str());
			return err;
		}
	}

	frame.timestamp.addTimestamp("Finishing processing data on CPU");

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/* Execute the ImageAnalyzer using the GPU.
 * Params:
 * 	@frame			the frame to be processed
 * 	@trackProcess	if we want to add timeStamps to the frame.timestamp
 * Returns the errCode
 */
errCode ImageAnalyzer::processImageOnGPU(Frame &frame, bool trackProcess)
{
	std::list<const ImageProcessor*>::iterator it = mImageProcessors.begin();
	errCode err = NO_ERROR;

	debug("Start to process frame on GPU\n");

	// Creates the cv::GpuMat structure
	cv::gpu::GpuMat frameData(frame.data);

	for(; it != mImageProcessors.end(); ++it){
		const ImageProcessor *imgProc = *it;

		// some log :)
		debug("Processing frame with: %s\n", imgProc->getName().c_str());

		if(trackProcess){
			frame.timestamp.addTimestamp("Processing with " + imgProc->getName());
		}

		err = imgProc->processData(frameData);
		if(err != NO_ERROR){
			debug("Some error occurred during the processing of the data %d\t%s\n",
					err, imgProc->getName().c_str());
			return err;
		}
	}

	// restores the data to the frame
	frame.data = frameData;

	frame.timestamp.addTimestamp("Finishing processing data on GPU");

	return NO_ERROR;
}


