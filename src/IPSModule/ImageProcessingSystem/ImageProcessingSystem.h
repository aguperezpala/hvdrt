/*
 * ImageProcessingSystem.h
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

#ifndef IMAGEPROCESSINGSYSTEM_H_
#define IMAGEPROCESSINGSYSTEM_H_

#include <QtGui/QWidget>
#include <QtGui/qwidget.h>

#include <string>

#include "DebugUtil.h"
#include "GlobalDefines.h"


class ImageProcessingSystem {
public:
	ImageProcessingSystem(const std::string &name = "",
			const std::string &info = "") :
				mName(name),
				mInfo(info)
	{

	}

	virtual ~ImageProcessingSystem();


	/* Returns the name of the IPS */
	const std::string &getName(void) const {return mName;}

	/* Returns the info associated to the IPS */
	const std::string &getInfo(void) const {return mInfo;}

	/* Check if the IPS have or not a additional Window info */
	virtual bool hasWindowInfo(void) const {return false;}

	/* Show the window info. Returns true on success / false on error */
	virtual bool showWindowInfo(QWidget *parent){return false;};

	/* Initialize the Image Processing System.
	 * Returns errorCode
	 */
	virtual errCode initialize(void) = 0;

	/* Executes the IPS and blocks until it finish.
	 * Params:
	 * 	@parent			The Parent Window
	 * Returns:
	 * 	errCode
	 */
	virtual errCode execute(QWidget *parent) = 0;

private:
	ImageProcessingSystem(const ImageProcessingSystem &);


private:
	std::string 		mName;
	std::string 		mInfo;

};

#endif /* IMAGEPROCESSINGSYSTEM_H_ */
