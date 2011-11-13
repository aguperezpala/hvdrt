/*
 * CoordsInterpreter.cpp
 *
 *  Created on: 23/10/2011
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

#include <opencv2/highgui/highgui.hpp>

#include "CoordsInterpreter.h"



/* function used to clear the data */
void CoordsInterpreter::clearData(void) const
{
	ASSERT(mData);
	for(int i = mData->size()-1; i >= 0; --i){
		mData->at(i).clear();
	}
}




CoordsInterpreter::CoordsInterpreter(const std::string &name) :
		ImageProcessor(name),
		mData(0),
		mFunctor(0)
{
	// TODO Auto-generated constructor stub

}

CoordsInterpreter::~CoordsInterpreter() {
	// TODO Auto-generated destructor stub
}

/* Sets the rectangle where it will be analyzed the image. */
void CoordsInterpreter::setAnalyzeZone(const cv::Rect &zone)
{
	mAnalyzeZone = zone;

	// if we have set the data before, we must resize it
	if(mData){
		if(mData->size() < mAnalyzeZone.width){
			mData->resize(mAnalyzeZone.width + 1);
		}
	}

	cv::namedWindow("coordsInterpreterWin");
}

/* Sets the Data where it will be stored the results.
 * REQUIRES:
 * 		data		!= 0
 * 		data.size	== zone.width
 */
void CoordsInterpreter::setData(Data *data)
{
	ASSERT(data);

	if(mData){
		debug("Warning, we are setting two times the Data\n");
	}

	if(data->size() < mAnalyzeZone.width){
		data->resize(mAnalyzeZone.width + 1);
	}
	mData = data;
}


// Proccess the data on the CPU
errCode CoordsInterpreter::processData(cv::Mat &image) const
{
	ASSERT(mData);

	if(image.channels() != 1){
		debug("Warning: we are assuming that there are only 1 channel\n");
		ASSERT(false);
	}

	// Clears the old data
	clearData();

	// set the coords to analyze
	int l = mAnalyzeZone.tl().y;
	int nl = l + mAnalyzeZone.height;

	int c = mAnalyzeZone.tl().x;
	int beginColumn = mAnalyzeZone.tl().x;
	int nc = c + mAnalyzeZone.width;

	int brY = mAnalyzeZone.br().y;

	// Check that we are inside of the image
	if((nl > image.rows) || (c  > image.cols)){
		debug("Error: Analyze Zone is wrong\n");
		ASSERT(false);
	}

	uchar* data = image.data + l*image.step + c*image.elemSize();
	// for all pixels
	for (; l < nl; ++l) {
		for (c = beginColumn; c < nc; ++c){
			if (*data == 0) {
				// go to the next column
				data += image.elemSize();
				continue;
			}
			// we have a point to store
			(mData->at(c - beginColumn)).push_back(brY - l);
			data += image.elemSize();
		}

		// go to the next row/line
		data = image.data + l*image.step + beginColumn*image.elemSize();
	}

	cv::imshow("coordsInterpreterWin", image);

	// call the callBack functor to advise the data is ready
	if(mFunctor){
		(*mFunctor)();
	}


	return NO_ERROR;
}

// Process the data on the GPU
errCode CoordsInterpreter::processData(cv::gpu::GpuMat &data) const
{
	// by now we not support this function
	return FEATURE_NOT_SUPPORTED;
}

