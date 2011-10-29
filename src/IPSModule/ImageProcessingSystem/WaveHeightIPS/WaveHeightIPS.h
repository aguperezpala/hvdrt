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

#include <QtGui/qwidget.h>

#include "DebugUtil.h"
#include "ImageProcessingSystem.h"

#include "ImageGenerator.h"
#include "DeviceCalibrator.h"
#include "ImageAnalyzer.h"


#define WHIPS_NAME		"Wave Height Calculator Image Processing System"
#define WHIPS_INFO		"System used to calculate the height of a wave using" \
		" a real time camera capturing system. TODO: cambiar esta info :)"




class WaveHeightIPS : public ImageProcessingSystem {
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
	virtual errCode execute(QWidget *parent);

private:
	/* Configures the device */
	bool configureDevice(void);

	/* Calibrates the camera */
	bool calibrateCamera(void);

	/* Select the ImageProcessor configuration */


private:
	QWidget					*mParentWidget;
	// The Image generator
	ImageGenerator			mImageGenerator;
	// The result of the device calibrator
	ImageProcessor			*mImgProcCalibrator;
	// The Handler of image processors
	ImageAnalyzer			mImgAnalizer;

};

#endif /* WAVEHEIGHTIPS_H_ */
