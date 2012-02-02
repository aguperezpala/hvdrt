/*
 * WaveHeightIPS.cpp
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

#include "IAnalyzer.h"
#include "WaveHeightIPFactory.h"

#include "WaveHeightIPS.h"

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::createImgProcessors(void) throw (WaveHeightException)
{
	// check if we already was initialized
	if(!mImageProcessors.empty()){
		throw WaveHeightException(ALREADY_INITIALIZED, "The WaveHeightIPS was "
				"already initialized");
		// clear and FREE all the memory here if we will continue
	}

	// clear the Image analizyer
	if(mImgAnalyzer.get()){
		mImgAnalyzer->removeAllProcessors();
	} else {
		mImgAnalyzer.reset(new ImageAnalyzer());
	}


	// we create the factory and construct all the IP
	WaveHeightIPFactory factory;

//	IP_RECTIFIER,		// The IP used to rectify the image
//			IP_CLIPPING,		// The IP used to clip the image
//			IP_BORDER_DETECTOR,	// Canny
//			IP_WH_ANALYZER,		// The wave height (pixel) analyzer

	// create the Rectifier
	ImageProcessor *rectifier = factory.getImageProcessor(WaveHeightIPFactory::IP_RECTIFIER);
	if(!rectifier){
		throw WaveHeightException(INTERNAL_ERROR, "Couldn't create the Rectifier IP");
	}
	mImgAnalyzer->addNewProcessor(rectifier);
	mImageProcessors[rectifier->getName()] = rectifier;

	// create the Clipping
	ImageProcessor *clipping = factory.getImageProcessor(WaveHeightIPFactory::IP_CLIPPING);
	if(!clipping){
		throw WaveHeightException(INTERNAL_ERROR, "Couldn't create the Clipping IP");
	}
	mImgAnalyzer->addNewProcessor(clipping);
	mImageProcessors[clipping->getName()] = clipping;

	// create the Border Detector
	ImageProcessor *bd = factory.getImageProcessor(WaveHeightIPFactory::IP_BORDER_DETECTOR);
	if(!bd){
		throw WaveHeightException(INTERNAL_ERROR, "Couldn't create the BorderDetector IP");
	}
	mImgAnalyzer->addNewProcessor(bd);
	mImageProcessors[bd->getName()] = bd;

	// create the Analyzer
	ImageProcessor *analyzer = factory.getImageProcessor(WaveHeightIPFactory::IP_WH_ANALYZER);
	if(!analyzer){
		throw WaveHeightException(INTERNAL_ERROR, "Couldn't create the Analyzer IP");
	}
	mImgAnalyzer->addNewProcessor(analyzer);
	// set the analyzer
	mIPAnalyzer = analyzer;
	mImageProcessors[analyzer->getName()] = analyzer;

	// everything was ok. returns
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::getAnalyzedData(void)
{
	ASSERT(mIPAnalyzer);
	// returns the data analyzed
	if(mIPAnalyzer->getParameter(IAnalyzer::GET_TIME, mAnalyzedData.time) != NO_ERROR ||
			mIPAnalyzer->getParameter(IAnalyzer::GET_HEIGHT, mAnalyzedData.height) != NO_ERROR ||
			mIPAnalyzer->getParameter(IAnalyzer::GET_PIXEL_POS, mAnalyzedData.pixelPos) != NO_ERROR)
	{
		debug("Error: Invalid IPAnalyzer\n");
		ASSERT(false);
	}
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::saveDataToFile(void)
{
	if(mOutFile.is_open()){
		mOutFile << mAnalyzedData.time << "," << mAnalyzedData.height << std::endl;
	}


}

////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
	// this function will save the frame and pixel choosed as the "wave height"
void WaveHeightIPS::savePixelDetected(void)
{
	static std::ofstream out;
	static double v = -1.0;
	static bool pointsSaved = false;

	if(!out.is_open()){
		out.open("debug_info.txt");
	}
	if(!pointsSaved){
		ImageProcessor *ip = getImageProcessor("PerspectiveRectifier");
		ASSERT(ip);
		double value;

		ip->getParameter(0, value);
		out << value << "\t";
		ip->getParameter(1, value);
		out << value << "\t";
		ip->getParameter(2, value);
		out << value << "\t";
		ip->getParameter(3, value);
		out << value << "\t";
		ip->getParameter(4, value);
		out << value << "\t";
		ip->getParameter(5, value);
		out << value << "\t";
		ip->getParameter(6, value);
		out << value << "\t";
		ip->getParameter(7, value);
		out << value << "\n";
		pointsSaved = true;
	}

	if(v < 0.0){
		getImageProcessor("MiddlePointClipping")->getParameter(0, v);
	}



	out << mImageGenerator.getDevice()->get(CV_CAP_PROP_POS_FRAMES) <<
			"\t" << static_cast<int>(mAnalyzedData.pixelPos) << "\t" <<
			static_cast<int>(v) << std::endl;

}
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
WaveHeightIPS::WaveHeightIPS() :
	ImageProcessingSystem(WHIPS_NAME, WHIPS_INFO),
	mIPAnalyzer(0),
	mOutFilename("out.txt"),
	mCallback(0),
	mImgAnalyzer(0),
	mProcType(CPU_PROCESS),
	mTrack(false),
	mRunning(false)
{

}

////////////////////////////////////////////////////////////////////////////////
WaveHeightIPS::~WaveHeightIPS()
{
	ImageProcessorMap::iterator it = mImageProcessors.begin(),
			endIt = mImageProcessors.end();
	for(; it != endIt; ++it){
		delete it->second;
	}

	mOutFile.close();

}



////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightIPS::initialize(void)
{
	try {
		createImgProcessors();
	} catch(WaveHeightException &e) {
		debug("Some error ocurr: %s\n", e.info.c_str());
		return e.code;
	}

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightIPS::execute(void)
{
	// check if we have initialized
	if(mImageProcessors.empty() || !mImgAnalyzer.get()){
		debug("Error: The IPS was not initialized\n");
		return INCOMPLETE_CONFIGURATION;
	}

	// open with default filename
	if(!mOutFile.is_open()){
		mOutFile.open(mOutFilename.c_str(), std::ios::out);

		if(!mOutFile.is_open()){
			debug("Error while opening %s file\n", mOutFilename.c_str());
			return INTERNAL_ERROR;
		}

		// write initial values
		mOutFile << "Tiempo (segudos),Altura (metros)" << std::endl;
	}

	mRunning = true;

	errCode result = NO_ERROR;
	Frame frame;
	debug("Starting main loop\n");
	while(mRunning && (result == NO_ERROR)){
		result = mImageGenerator.captureFrame(frame);
		if(result != NO_ERROR){
			debug("Error while getting the frame\n");
			break;
		}
		switch(mProcType){
		case CPU_PROCESS:
			result = mImgAnalyzer->processImageOnCPU(frame, mTrack);
			break;

		case GPU_PROCESS:
			result = mImgAnalyzer->processImageOnGPU(frame, mTrack);
			break;

		default:
			ASSERT(false);
		}

		// get the analyzed data
		getAnalyzedData();

		// save the data to the file
		saveDataToFile();

		// we have finish processing the frame, advice by the callback
		if(mCallback){
			(*mCallback)(mAnalyzedData);
		}

#ifdef DEBUG
		savePixelDetected();
#endif

	}

	// close the file
	mOutFile.close();

	mRunning = false;
	return result;
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::stop(void)
{
	mRunning = false;
}

////////////////////////////////////////////////////////////////////////////////
ImageProcessor *WaveHeightIPS::getImageProcessor(const std::string &name)
{
	ImageProcessorMap::iterator it = mImageProcessors.find(name);
	if(it == mImageProcessors.end()){
		debug("Warning: No ImageProcessor with name %s was found\n", name.c_str());
		return 0;
	}

	return it->second;
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::setCallBackFunctor(CallBFunctor *cb)
{
	if(!cb){
		debug("Warning, callback null\n");
	}
	if(mCallback){
		debug("Warning: callback already set\n");
	}

	// overwrite the lastone
	mCallback = cb;
}


////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::setOutFilename(const std::string &fname)
{
	mOutFilename = fname;
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::noSaveToFile(void)
{
	mOutFilename = "";
	// close the actual file
	mOutFile.close();
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::setTrackingMode(bool track)
{
	mTrack = track;;
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::setProcessMode(ProcessType mode)
{
	mProcType = mode;
}
