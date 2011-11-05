/*
 * WaveHeightIPS.h
 *
 *  Created on: 29/10/2011
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

#ifndef WAVEHEIGHTIPS_H_
#define WAVEHEIGHTIPS_H_


#include "DebugUtil.h"
#include "ImageProcessingSystem.h"
#include "FrameListener.h"

#include "ImageGenerator.h"
#include "DeviceCalibrator.h"
#include "ImageAnalyzer.h"


#define WHIPS_NAME		"Wave Height Calculator Image Processing System"
#define WHIPS_INFO		"System used to calculate the height of a wave using" \
		" a real time camera capturing system. TODO: cambiar esta info :)"




class WaveHeightIPS : public ImageProcessingSystem {
public:
	typedef enum {
		CPU_PROCESS,
		GPU_PROCESS,
	} ProcessType;


private:
	/* Auxiliar class used to receive the frames and process it */
	class FrameProcessor : public FrameListener {
	public:
		FrameProcessor() :
			mProcType(CPU_PROCESS),
			mTrack(false)
			{}
		~FrameProcessor(){};

		/* Set if we want to process the frame in GPU or CPU */
		void setProcType(ProcessType t){mProcType = t;}
		ProcessType getProcType(void) const {return mProcType;}

		/* sets if we want to track the process (timestamps) */
		void setTrackingMode(bool track){mTrack = track;}
		bool getTrackingMode(void) const {return mTrack;}

		/* sets the image analyzer to be used */
		void setImageAnalyzer(ImageAnalyzer *ia)
		{
			ASSERT(ia);
			mImgAnalizer = ia;
		}

		/* Returns the image analyzer */
		ImageAnalyzer *getImageAnalyzer(void) {return mImgAnalyzer;}

		errCode processFrame(Frame &frame)
		{
			errCode result = INTERNAL_ERROR;

			ASSERT(mImgAnalizer);
			switch(mProcType){
			case CPU_PROCESS:
				result = mImgAnalizer->processImageOnCPU(frame, mTrack);
				break;

			case GPU_PROCESS:
				result = mImgAnalizer->processImageOnGPU(frame, mTrack);
				break;

			default:
				ASSERT(false);
			}

			return result;
		}


	private:
		ProcessType			mProcType;
		ImageAnalyzer		*mImgAnalyzer;
		bool				mTrack;


	};

public:
	WaveHeightIPS();
	virtual ~WaveHeightIPS();

	/* Initialize the Image Processing System.
	 * Returns errorCode
	 */
	virtual errCode initialize(void);

	/* Executes the IPS and blocks until it finish.
	 * Params:
	 * 	@parent			The Parent Window
	 * Returns:
	 * 	errCode
	 */
	virtual errCode execute(void);

	/* Returns the ImageGenerator instance */
	ImageGenerator *getImageGenerator(void) {return &mImageGenerator;}

	/* Sets the ImageProcessor calibrator.
	 * Requires:
	 * 	calibrator		!= 0
	 * This class do not free any memory
	 */
	void setImgProcCalibrator(ImageProcessor *calibrator);

	/* Returns the ImageProc calibrator */
	ImageProcessor *getCalibrator(void) const {return mImgProcCalibrator;}

		/* Sets the ImageAnalizer to be used
	 * Requires:
	 * 	analyzer	!= 0
	 * This class do not free the memory
	 */
	void setImgAnalyzer(ImageAnalyzer *analyzer);

	/* Returns the ImageAnalyzer */
	ImageAnalyzer *getImageAnalyzer(void) {return mFrameProc.getImageAnalyzer();}

	/* Set the timestamp and info tracking mode.
	 * @track	if true then it will track, if false it will not track
	 */
	void setTrackingMode(bool track);
	bool getTrackingMode(void) const {return mFrameProc.getTrackingMode();}

	/* Set the process mode (GPU | CPU).
	 * @mode	The mode to execute
	 */
	void setProcessMode(ProcessType mode);
	ProcessType getProcessMode(void) const {return mFrameProc.getProcType();}


private:
	// The Image generator
	ImageGenerator			mImageGenerator;
	// The result of the device calibrator
	ImageProcessor			*mImgProcCalibrator;
	// The frame processor used
	FrameProcessor			mFrameProc;

};

#endif /* WAVEHEIGHTIPS_H_ */
