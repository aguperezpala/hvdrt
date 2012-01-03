/*
 * WaveHeightAnalyzer.cpp
 *
 *  Created on: 03/01/2012
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

#include "WaveHeightAnalyzer.h"

WaveHeightAnalyzer::WaveHeightAnalyzer() :
IAnalyzer("WaveHeightAnalyzer")
{
	// TODO Auto-generated constructor stub

}

WaveHeightAnalyzer::~WaveHeightAnalyzer() {
	// TODO Auto-generated destructor stub
}


// Set a parameter to the ImageProcessor, used to configure the IP.
errCode WaveHeightAnalyzer::setParameter(int param, double value)
{
	switch(param){
	case TLX_ZONE:
		mTLZone.x = value;
		break;
	case TLY_ZONE:
		mTLZone.y = value;
		break;
	case BRX_ZONE:
		mBRZone.x = value;
		break;
	case BRY_ZONE:
		mBRZone.y = value;
		break;
	case SIZE_RELATION:
		mSizeRelation = value;
		break;

	default:
		return INVALID_PARAM;
	}
	return NO_ERROR;
}

// Gets a parameter value returned by param
errCode WaveHeightAnalyzer::getParameter(int param, double &value) const
{
	switch(param){
	case TLX_ZONE:
		value = mTLZone.x;
		break;
	case TLY_ZONE:
		value = mTLZone.y;
		break;
	case BRX_ZONE:
		value = mBRZone.x;
		break;
	case BRY_ZONE:
		value = mBRZone.y;
		break;
	case SIZE_RELATION:
		value = mSizeRelation;
		break;

	default:
		return INVALID_PARAM;
	}
	return NO_ERROR;
}

// Proccess the data on the CPU
errCode WaveHeightAnalyzer::processData(cv::Mat &data) const
{

}

// Process the data on the GPU
errCode WaveHeightAnalyzer::processData(cv::gpu::GpuMat &data) const
{
	return FEATURE_NOT_SUPPORTED;
}
