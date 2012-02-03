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
#include "GlobalDefines.h"
#include "Frame.h"
#include "FrameEmitter.h"


class ImageGenerator : public FrameEmitter
{
public:

	typedef enum {
		NONE,
		CAMERA_DEV,
		VIDEO_DEV,
	} DeviceType;
public:
	ImageGenerator();
	virtual ~ImageGenerator();

	/* Creates a device from a camera and automatically sets the device to
	 * this object
	 * Params:
	 * 		@device		the device ID (0 as default)
	 * Returns:
	 * 		true		on success
	 * 		false		otherwise
	 */
	virtual bool createDevice(int device = 0);

	/* Creates a device from a video and automatically sets the device to this
	 * object
	 * Params:
	 * 		@videoPath	the path to the video file
	 * Returns:
	 * 		true		on success
	 * 		false		otherwise
	 */
	virtual bool createDevice(const std::string &videoPath);

	/* Check what type of device is */
	virtual DeviceType getDeviceType(void) const;

	/* Destroy the actual device */
	void destroyDevice(void);

	/* Sets the device.
	 * Requires:
	 * 		No device set before.
	 * 	After calling this function the device d must not be deleted
	 */
	virtual bool setDevice(cv::VideoCapture *d);

	/* Returns the device.
	 * DO NOT DELETE the device */
	virtual cv::VideoCapture *getDevice(void) const {return mCapturer;}

	/* Capture one frame and returns the frame in result.
	 * Returns:
	 * 	errCode
	 */
	virtual errCode captureFrame(Frame &result);

	/* Returns the last image frame captured */
	virtual const cv::Mat &getLastFrameCaptured(void) const {return mFrameData;}

	/* This is the main function. Start to capture frames an call the FrameListeners
	 * This will create the threadpool/threads if it is necessary.
	 * Is a blocking function
	 * RETURNS:
	 * 		errCode
	 */
	virtual errCode startGenerating(void);

	/* Stops generating frames */
	virtual void stopGenerating(void);


private:
	cv::VideoCapture		*mCapturer;
	cv::Mat					mFrameData;
	bool					mStop;

};

#endif /* IMAGEGENERATOR_H_ */
