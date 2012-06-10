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

// TODO: for windows we have to install this library
#include <pthread.h>


#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "Frame.h"
#include "FrameEmitter.h"
#include "FrameBuffer.h"

#define IG_VIDEO_OUT_FNAME		"out.avi"
#define IG_VIDEO_OUT_FPS		25.0

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

	/**
	 * Configure the image generator to use buffering.
	 * @param	nf		The number of frames to use in the buffering
	 */
	void configureBuffering(int nf = 5);

	/**
	 * Configure the image generator to write the images into a video file
	 * @param	fn		The filename to output
	 * @param	fps		The frame rate to save the video
	 *
	 * @note	The video will be saved with the format
	 * CV_FOURCC('M','J','P','G')
	 */
	void setVideoOut(const std::string &fname = IG_VIDEO_OUT_FNAME,
			double fps = IG_VIDEO_OUT_FPS);

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

	/**
	 * This is a new "capturing mode", the buffering mode, this is a blocking
	 * function that will start to capture frames and will put all the frames
	 * into the FrameBuffer, and the can be retrieved from "getBufferedFrame"
	 * @returns		errCode
	 */
	virtual errCode startBuffering(void);
	virtual void stopBuffering(void);

	/**
	 * Returns some frame captured using "bufferingMode".
	 * If there are no frame available, then this function blocks until some
	 * new frame are available.
	 * @return	frame		If some error occurs, 0 is returned
	 * @note	Once the frame is already processed it must be returned to the
	 * 			buffer again (calling frameRady(Frame)).
	 */
	virtual Frame *getBufferedFrame(void);

	/**
	 * Enqueues the frame again in the buffer to be used again in the buffering
	 * cycle
	 * @param	f		The frame already processed
	 */
	virtual void frameReady(Frame *f);


private:
	cv::VideoCapture		*mCapturer;
	cv::Mat					mFrameData;
	bool					mStop;
	DeviceType				mDevType;

	// Save the video into hard disc
	cv::VideoWriter			mWriter;
	std::string				mOutVideo;
	double					mOutFrameRate;

	// sincronization system
	bool					mBuffering;
	FrameBuffer				mBuffer;
	FrameBuffer				mProcessedFrames;
};

#endif /* IMAGEGENERATOR_H_ */
