/*
 * ImageAnalyzer.h
 *
 *  Created on: 13/09/2011
 *      Author: agustin
 */

#ifndef IMAGEANALYZER_H_
#define IMAGEANALYZER_H_


#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>

#include <list>
#include <algorithm>

#include "DebugUtil.h"
#include "Frame.h"
#include "GlobalDefines.h"
#include "ImageProcessor.h"


class ImageAnalyzer
{
public:
	ImageAnalyzer();
	virtual ~ImageAnalyzer();

	/* Add a new image processor to the queue. The ImageProcessor will be
	 * executed in the order that they will be put here
	 */
	virtual void addNewProcessor(const ImageProcessor *ip);
	virtual void removeProcessor(const ImageProcessor *ip);
	virtual void removeAllProcessors(void);
	virtual const std::list<const ImageProcessor*> &getProcessors(void) const
	{
		return mImageProcessors;
	}

	/* Execute the ImageAnalyzer using the CPU.
	 * Params:
	 * 	@frame			the frame to be processed
	 * 	@trackProcess	if we want to add timeStamps to the frame.timestamp
	 * Returns the errCode and the data processed in frame.data
	 */
	virtual errCode processImageOnCPU(Frame &frame, bool trackProcess = false);

	/* Execute the ImageAnalyzer using the GPU.
	 * Params:
	 * 	@frame			the frame to be processed
	 * 	@trackProcess	if we want to add timeStamps to the frame.timestamp
	 * Returns the errCode and the data processed in frame.data
	 */
	virtual errCode processImageOnGPU(Frame &frame, bool trackProcess = false);

private:
	std::list<const ImageProcessor*>	mImageProcessors;

};


#endif /* IMAGEANALYZER_H_ */
