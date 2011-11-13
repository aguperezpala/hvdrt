/*
 * ImgPointPainter.h
 *
 *  Created on: 13/11/2011
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

#ifndef IMGPOINTPAINTER_H_
#define IMGPOINTPAINTER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "ImageProcessor.h"
#include "DataProcessor.h"

class ImgPointPainter  : public ImageProcessor{
public:
	ImgPointPainter(const std::string &name = "ImgPointPainter"):ImageProcessor(name)
	{}
	virtual ~ImgPointPainter(){};

	void setDataProcessor(DataProcessor *data) {
		ASSERT(data);
		mData = data;
	}


	// Process the data on the CPU
	errCode processData(cv::Mat &data) const
	{
		ASSERT(mData);
		float v = mData->getLastHeightCalculated();

		paintPixel(data, data.cols/2 ,v);

		return NO_ERROR;
	}

	// Process the data on the GPU
	errCode processData(cv::gpu::GpuMat &data) const
	{
		return FEATURE_NOT_SUPPORTED;
	}

private:
	void paintPixel(cv::Mat &img, int c, int r) const
	{
		//cv::cvtColor(img, img, CV_BGR2RGB);
		int lc = c - 10;
		int rc = c + 10;
		int tr = r - 10;
		int br = r + 10;

		for(int i = lc; i < rc; ++i){
			for(int j = tr; j < br; ++j){
				uchar *data = img.data + j*img.step + i*img.elemSize();
				*data = 200;
			}
		}


	}
private:
	DataProcessor		*mData;
};

#endif /* IMGPOINTPAINTER_H_ */
