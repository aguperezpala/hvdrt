/*
 * CameraResolution.cpp
 *
 *  Created on: 22/09/2011
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


#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cstdlib>

int main(int argc, char**args)
{

	if(argc != 3){
		std::cout << "USE params: width height \n";
		return 1;
	}


	std::cout << "Opening the camera\n";
	cv::VideoCapture cap(0); // open the default camera
	if(!cap.isOpened()){
		// check if we succeeded
		std::cout << "There are no cammera\n";
		return -1;
	}

	std::cout << "Getting actual camera frame size\n";
	std::cout << "Width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
	std::cout << "Height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;

	int width = atoi(args[1]);
	int height = atoi(args[2]);
	std::cout << "Trying to set this option to the camera\n";

	/* NOTE: En el archivo cap_v4l.cpp de openCV en la funcion icvSetPropertyCAM_V4L
	 * dice que hay que setear 2 veces la resolucion para que tenga exito ya que
	 * estan declaradas las variables como statics.
	 * Para verificar que realmente se pudo cambiar la resolucion debemos simplemente
	 * ver el valor obtenido con cap.get() y no el valor de retorno de la funcion set
	 */
	cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);


	if(cap.set(CV_CAP_PROP_FRAME_WIDTH, width) == false){
		std::cout << "Error setting the width: " << width << std::endl;
		return 1;
	}
	if(cap.set(CV_CAP_PROP_FRAME_HEIGHT, height) == false){
		std::cout << "Error setting the height: " << height << std::endl;
		return 1;
	}

	std::cout << "Width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
	std::cout << "Height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;



    cv::Mat edges;
    cv::namedWindow("edges",1);
    for(;;)
    {
    	cv::Mat frame;

        cap >> frame; // get a new frame from camera
        cv::imshow("edges", frame);
        if(cv::waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
