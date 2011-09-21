/*
 * test.cpp
 *
 *  Created on: 16/09/2011
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


#include <iostream>
#include <string>

#include "DebugUtil.h"
#include "ImageGenerator.h"
#include "ImageAnalyzer.h"
#include "AuxClasses.h"
#include "GlobalDefines.h"




static void testMethod1()
{
	ImageGenerator ig;
	TestFrameListener1 l;

	cv::namedWindow("TestFrameListener1",1);

	ASSERT(ig.createDevice("/home/agustin/facu/tesis/muestreo_videos_lab/video_test.avi"));

	ig.addNewListener(&l);

	std::cout << "testMethod1: " << ig.startGenerating() << std::endl;

}

static void testMethod2()
{
	ImageGenerator ig;
	TestFrameListener2 l;

	cv::namedWindow("TestFrameListener2",1);
	ASSERT(ig.createDevice("/home/agustin/facu/tesis/muestreo_videos_lab/video_test.avi"));

	ig.addNewListener(&l);

	std::cout << "testMethod2: " << ig.startGenerating() << std::endl;


}


int main(void)
{
	testMethod1();
	testMethod2();

	return 0;
}
