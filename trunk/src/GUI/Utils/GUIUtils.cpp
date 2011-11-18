/*
 * GUIUtils.cpp
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


#include <qmessagebox.h>

#include "GUIUtils.h"

void GUIUtils::showMessageBox(const std::string &text)
{
	QMessageBox msg(0);
	msg.setText(text.c_str());
	msg.exec();

}



/* Transforms an opencv Mat image to a QT image */
bool GUIUtils::IplImage2QImage(const cv::Mat &iplImgMat, QImage &ref)
{
	IplImage iplImg = iplImgMat;
	int h = iplImg.height;
	int w = iplImg.width;
	int channels = iplImg.nChannels;
	QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
	char *data = iplImg.imageData;

	for (int y = 0; y < h; y++, data += iplImg.widthStep)
	{
		for (int x = 0; x < w; x++)
		{
			char r, g, b, a = 0;
			if (channels == 1)
			{
				r = data[x * channels];
				g = data[x * channels];
				b = data[x * channels];
			}
			else if (channels == 3 || channels == 4)
			{
				r = data[x * channels + 2];
				g = data[x * channels + 1];
				b = data[x * channels];
			}

			if (channels == 4)
			{
				a = data[x * channels + 3];
				qimg->setPixel(x, y, qRgba(r, g, b, a));
			}
			else
			{
				qimg->setPixel(x, y, qRgb(r, g, b));
			}
		}
	}
	ref = *qimg;
	delete qimg;


	return true;
}
