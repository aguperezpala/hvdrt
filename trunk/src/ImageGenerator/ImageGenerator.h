/*
 * ImageGenerator.h
 *
 *  Created on: 12/09/2011
 *      Author: agustin
 */

#ifndef IMAGEGENERATOR_H_
#define IMAGEGENERATOR_H_


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "DebugUtil.h"
#include "Frame.h"
#include "FrameListener.h"


class ImageGenerator
{
public:
	ImageGenerator();
	virtual ~ImageGenerator(){};

	/* Sets the device */
	virtual void setDevice(cv::VideoCapture *d);

	/* Returns the device */
	virtual cv::VideoCapture *getDevice(void) const {return mCapturer;}

	/* Adds a new FrameListener */
	virtual bool addNewFrameListener(FrameListener *fl);

	/* Removes a FrameListener if exists */
	virtual void removeFrameListener(FrameListener *fl);

	/* Returns true if we are generating image from files */
	virtual bool isGenFromFile(void) const;

	/* This is the main function. Start to capture frames an call the FrameListeners
	 * This will create the threadpool/threads if it is necessary.
	 * Is a blocking function
	 * RETURNS:
	 * 		errCode
	 */
	virtual int startGenerating(void);

	/* Stops generating frames */
	virtual void stopGenerating(void);


private:
	cv::VideoCapture		*mCapturer;
};

#endif /* IMAGEGENERATOR_H_ */
