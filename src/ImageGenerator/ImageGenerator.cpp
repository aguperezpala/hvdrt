/*
 * ImageGenerator.cpp
 *
 *  Created on: 12/09/2011
 *      Author: agustin
 */

#include <unistd.h>

#include "ImageGenerator.h"



////////////////////////////////////////////////////////////////////////////////
ImageGenerator::ImageGenerator() :
	mCapturer(0)
{
	// TODO Auto-generated constructor stub

}

////////////////////////////////////////////////////////////////////////////////
ImageGenerator::~ImageGenerator()
{
	if(mCapturer){
		delete mCapturer;
	}
}

////////////////////////////////////////////////////////////////////////////////
/* Creates a device from a camera and automatically sets the device to
 * this object
 * Params:
 * 		@device		the device ID (0 as default)
 * Returns:
 * 		true		on success
 * 		false		otherwise
 */
bool ImageGenerator::createDevice(int device)
{
	if(mCapturer){
		debug("There already exists a Device\n");
		return false;
	}

	mCapturer = new cv::VideoCapture(device);
	if(!mCapturer->isOpened()){
		delete mCapturer; mCapturer = 0;
		debug("Problem opening the device: %d\n", device);
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/* Creates a device from a video and automatically sets the device to this
 * object
 * Params:
 * 		@videoPath	the path to the video file
 * Returns:
 * 		true		on success
 * 		false		otherwise
 */
bool ImageGenerator::createDevice(const std::string &videoPath)
{
	if(mCapturer){
		debug("There already exists a Device\n");
		return false;
	}

	mCapturer = new cv::VideoCapture(videoPath);
	mCapturer->open(videoPath);
	if(!mCapturer->isOpened()){
		delete mCapturer; mCapturer = 0;
		debug("Problem opening the file: %s\n", videoPath.c_str());
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/* Check what type of device is */
ImageGenerator::DeviceType ImageGenerator::getDeviceType(void) const
{
	if(!mCapturer){
		return NONE;
	}

	// check if we can count frames, if not, there is a webcam
	if(mCapturer->get(CV_CAP_PROP_FRAME_COUNT) == 0.0){
		return CAMERA_DEV;
	} else {
		return VIDEO_DEV;
	}

	return NONE;
}

/* Destroy the actual device */
void ImageGenerator::destroyDevice(void)
{
	if(!mCapturer){
		debug("Warning: Destroying a non constructed device\n");
		return;
	}
	// destroy the device
	mCapturer->release();
	delete mCapturer;
	mCapturer = 0;
}

////////////////////////////////////////////////////////////////////////////////
/* Sets the device.
 * Requires:
 * 		No device set before.
 * 	After calling this function the device d must not be deleted
 */
bool ImageGenerator::setDevice(cv::VideoCapture *d)
{
	ASSERT(d);
	ASSERT(mCapturer == 0);
	mCapturer = d;

	if(!d->isOpened()){
		debug("The device set is not opened\n");
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
errCode ImageGenerator::captureFrame(Frame &result)
{
	if(!mCapturer){
		debug("Error: No capturer created\n");
		return DEVICE_NOT_SET;
	}

	if(!mCapturer->isOpened()){
		return DEVICE_NOT_WORKING;
	}

	// TODO: here we have to check the last time that this function was call and
	// see the device fps and wait for a while to let the device update the
	// buffer with the new image. by the moment we will wait only for 10
//	cv::waitKey(10);

	if(!mCapturer->read(mFrameData)){
		return CAPTURER_ERROR;
	}
	// avoid the reference counting to be 0
	result.data = mFrameData;

	// everything ok
	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/* This is the main function. Start to capture frames an call the FrameListeners
 * This will create the threadpool/threads if it is necessary.
 * Is a blocking function
 * RETURNS:
 * 		errCode
 */
errCode ImageGenerator::startGenerating(void)
{
	if(!mCapturer){
		return DEVICE_NOT_SET;
	}

	if(!mCapturer->isOpened()){
		return DEVICE_NOT_WORKING;
	}

	// TODO: configure the camera velocity, frame sizes and other stuff
	double frameRate = mCapturer->get(CV_CAP_PROP_FPS);
	if(frameRate == 0){
		// set as default 30fps
		frameRate = 30;
	}
	int delay = 1000.0/frameRate;
	double tm1 = 0, tm2 = 0;

	// TODO: here we have to see how we can optimize the frames grab velocity
	// using two buffers (cv::Mat) and swaping it, or a static Frames queue.


	// start to capture
	mStop = false;
	while(!mStop){

		// get the actual time
		tm1 = Timestamp::getNowTimeStamp();

		if(!mCapturer->read(mFrameData)){
			// TODO: probably this means that there are no more frames
			return CAPTURER_ERROR;
		}

		// set the data to own frame structure
		Frame frame;
		frame.data = mFrameData;

		// emmit the event to all the frame listeners
		emmitFrame(frame);

		tm2 = Timestamp::getNowTimeStamp();

		// TODO: get the time between the get frame and the now and delay
		tm2 = tm2 - tm1;
		tm2 *= 1000.0;

		// delay to get the next frame
		//usleep(static_cast<useconds_t>(delay - static_cast<int>(tm2))*1000);
		cv::waitKey(10);
	}

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/* Stops generating frames */
void ImageGenerator::stopGenerating(void)
{
	mStop = true;
}
