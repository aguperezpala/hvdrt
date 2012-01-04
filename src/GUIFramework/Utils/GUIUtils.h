/*
 * GUIUtils.h
 *
 *  Created on: 05/11/2011
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

#ifndef GUIUTILS_H_
#define GUIUTILS_H_

#include <qimage.h>

#include <opencv2/core/core.hpp>

#include <string>

class GUIUtils {
public:
	static void showMessageBox(const QString &text);

	/* Transforms an opencv Mat image to a QT image */
	static bool IplImage2QImage(const cv::Mat &iplImgMat, QImage &ref);
};

#endif /* GUIUTILS_H_ */
