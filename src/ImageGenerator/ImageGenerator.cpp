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
	mCapturer(0),
	mDevType(NONE),
	mOutFrameRate(0)
{

}

////////////////////////////////////////////////////////////////////////////////
ImageGenerator::~ImageGenerator()
{
	if(mCapturer){
		delete mCapturer;
	}

}


////////////////////////////////////////////////////////////////////////////////
void ImageGenerator::configureBuffering(int nf)
{
	ASSERT(nf > 0);
	mBuffer.freeAll();
	mBuffer.createFrames(nf);
}

////////////////////////////////////////////////////////////////////////////////
bool ImageGenerator::isUsingBuffering(void) const
{
	return (mBuffer.size() != 0) || (mProcessedFrames.size() != 0);
}

////////////////////////////////////////////////////////////////////////////////
void ImageGenerator::setVideoOut(const std::string &fname, double fps)
{
	mOutVideo = fname;
	mOutFrameRate = fps;
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
	mDevType = CAMERA_DEV;

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
	mDevType = VIDEO_DEV;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/* Check what type of device is */
ImageGenerator::DeviceType ImageGenerator::getDeviceType(void) const
{
	return mDevType;
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
	mDevType = NONE;
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
	// check if is a camera
	if(d->get(CV_CAP_PROP_GAIN) == 0.0){
		// is a video
		mDevType = VIDEO_DEV;
	} else {
		mDevType = CAMERA_DEV;
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
//		cv::waitKey(10);
	}

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/* Stops generating frames */
void ImageGenerator::stopGenerating(void)
{
	mStop = true;
}


////////////////////////////////////////////////////////////////////////////////
errCode ImageGenerator::startBuffering(void)
{
	mBuffering = true;

	if(mBuffer.size() <= 0){
		debugRED("Error: No buffering mode was configured\n");
		return INCOMPLETE_CONFIGURATION;
	}

	// try to open the video writer
	bool useWriter = false;
	if(!mOutVideo.empty()){
		cv::Size frameSize;
		if(mFrameData.empty()){
			Frame frame;
			errCode r = captureFrame(frame);
			if(r != NO_ERROR){
				return r;
			}
			ASSERT(!frame.data.empty());
			// get the frame size
			frameSize = frame.data.size();
		}
		if(!mWriter.open(mOutVideo, CV_FOURCC('M','J','P','G'),
				mOutFrameRate,frameSize)){
			debugRED("We cannot open the file %s to write it\n", mOutVideo.c_str());
			return INTERNAL_ERROR;
		}
		useWriter = true;
	}


	errCode err = INTERNAL_ERROR;
	while(mBuffering){
		// get new buffer
		Frame *f = mBuffer.getFrame();
		if(!f) {
			// we have finish, unlock the thread that is waiting for, in a very
			// ugly way
			mProcessedFrames.addNewAvailable(0);
			return CAPTURER_ERROR;
		}
		ASSERT(f);
		err = captureFrame(*f);
		if(err != NO_ERROR){
			debugRED("Some error occur: %d\n", err);
			return err;
		}
		// save into the video file
		if(useWriter) mWriter.write(f->data);

		// put the frame into available frames
		mProcessedFrames.addNewAvailable(f);
	}

	return NO_ERROR;
}
////////////////////////////////////////////////////////////////////////////////
void ImageGenerator::stopBuffering(void)
{
	mBuffering = false;
}

////////////////////////////////////////////////////////////////////////////////
Frame *ImageGenerator::getBufferedFrame(void)
{
	return mProcessedFrames.getFrame();
}

////////////////////////////////////////////////////////////////////////////////
void ImageGenerator::frameReady(Frame *f)
{
	ASSERT(f);
	mBuffer.addNewAvailable(f);
}
