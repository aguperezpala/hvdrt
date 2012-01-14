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

#include <string>
#include <map>
#include <auto_ptr.h>
#include <fstream>

#include "DebugUtil.h"
#include "ImageProcessingSystem.h"
#include "FrameListener.h"
#include "WaveHeightException.h"
#include "CallBFunctor.h"

#include "ImageGenerator.h"
//#include "DeviceCalibrator.h"
#include "ImageAnalyzer.h"


#define WHIPS_NAME		"Calculador de altura de ola"
#define WHIPS_INFO		"Sistema usado para calcular la altura de la ola usando" \
	" un sistema de captura en tiempo real o por medio de un archivo de video"



class WaveHeightIPS : public ImageProcessingSystem {
public:
	typedef enum {
		CPU_PROCESS,
		GPU_PROCESS,
	} ProcessType;

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

	/* Stops the ImageProcessingSystem execution */
	virtual void stop(void);

	/* Returns the ImageGenerator instance */
	ImageGenerator *getImageGenerator(void) {return &mImageGenerator;}

	/* Returns an ImageProcessor by name. In case that the ImageProcessor doesn't
	 * exists, 0 is returned.
	 * This function will be used to configure the different IP's of the IPS.
	 */
	ImageProcessor *getImageProcessor(const std::string &name);

	/* Set the CallBack functor used to advice that we have new data ready
	 * (analyzed) that could be retrieved by calling getDataAnalyzed()
	 */
	void setCallBackFunctor(CallBFunctor *);

	/* Returns the last analyzed data */
	inline const AnalyzedData &getDataAnalyzed(void) const {return mAnalyzedData;}

	/* Set the filename where we will save the data if we want to do it. If this
	 * function is never called then the results will be save in "out.txt"
	 * except when the "noSaveToFile()" function is called */
	void setOutFilename(const std::string &fname);

	/* Function used to avoid save data to out file */
	void noSaveToFile(void);

	/* Set the timestamp and info tracking mode.
	 * @track	if true then it will track, if false it will not track
	 */
	void setTrackingMode(bool track);
	bool getTrackingMode(void) const {return mTrack;}

	/* Set the process mode (GPU | CPU).
	 * @mode	The mode to execute
	 */
	void setProcessMode(ProcessType mode);
	ProcessType getProcessMode(void) const {return mProcType;}

private:
	/* Creates the ImageProcessors. On error, throws an exception */
	void createImgProcessors(void) throw (WaveHeightException);

	/* Get analyzed data */
	void getAnalyzedData(void);

	/* Save data to file */
	void saveDataToFile(void);


private:

	typedef std::auto_ptr<ImageAnalyzer>			ImageAnalyzerPtr;
	typedef std::map<std::string, ImageProcessor*> ImageProcessorMap;

	// The image processors
	ImageProcessorMap		mImageProcessors;
	// The IPAnalyzer
	ImageProcessor			*mIPAnalyzer;
	// The Image generator
	ImageGenerator			mImageGenerator;

	std::string				mOutFilename;
	CallBFunctor 			*mCallback;

	// Image analizyer
	ImageAnalyzerPtr		mImgAnalyzer;

	ProcessType				mProcType;
	bool					mTrack;

	std::ofstream			mOutFile;
	AnalyzedData			mAnalyzedData;

	bool					mRunning;




};

#endif /* WAVEHEIGHTIPS_H_ */
