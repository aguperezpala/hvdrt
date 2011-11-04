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

#include "WaveHeightIPS.h"

////////////////////////////////////////////////////////////////////////////////
WaveHeightIPS::WaveHeightIPS() :
ImageProcessingSystem(WHIPS_NAME, WHIPS_INFO)
{
	// adds the FrameProcessor to the ImageGeneratro
	mImageGenerator.addNewListener(&mFrameProc);
}

////////////////////////////////////////////////////////////////////////////////
WaveHeightIPS::~WaveHeightIPS()
{
	// TODO Auto-generated destructor stub
	mImageGenerator.removeFrameListener(&mFrameProc);
}



////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightIPS::initialize(void)
{


	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode WaveHeightIPS::execute(void)
{
	if(!mFrameProc.getImageAnalyzer()){
		debug("ERROR: No ImageAnalyzer was set\n");
		return INCOMPLETE_CONFIGURATION;
	}
	if(!mImgProcCalibrator){
		debug("Warning: There was no set the Calibrator Image Processor\n");
	} else {
		const std::list<const ImageProcessor*> &l = mFrameProc.getImageAnalyzer()->getProcessors();
		std::list<const ImageProcessor*>::const_iterator it =
				std::find(l.begin(), l.end(), mImgProcCalibrator);

		// we have to set the image processor if it wasn't set before
		if(it == l.end()){
			// add the imageProcessor to the list first
			std::list<const ImageProcessor*> nl = l;
			nl.pop_front(mImgProcCalibrator);
			mFrameProc.getImageAnalyzer()->addNewProcessors(nl);
		}
	}


	return mImageGenerator.startGenerating();
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::setImgProcCalibrator(ImageProcessor *calibrator)
{
	ASSERT(calibrator);

	if(mImgProcCalibrator){
		debug("Warning: A previous calibrator was set\n");
	}
	mImgProcCalibrator = calibrator;
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::setImgAnalyzer(ImageAnalyzer *analyzer)
{
	ASSERT(analyzer);

	if(mImgAnalizer){
		debug("Warning: A previous Analyzer was set\n");
	}

	mFrameProc.setImageAnalyzer(analyzer);

}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::setTrackingMode(bool track)
{
	mFrameProc.setTrackingMode(track);
}

////////////////////////////////////////////////////////////////////////////////
void WaveHeightIPS::setProcessMode(ProcessType mode)
{
	mFrameProc.setProcType(mode);
}
