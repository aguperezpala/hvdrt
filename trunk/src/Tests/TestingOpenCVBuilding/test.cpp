/*
 * test.cpp
 *
 *  Created on: 02/09/2011
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

#include <cv.h>
#include <highgui.h>
#include <iostream>


using namespace cv;

int main(int, char**)
{
    VideoCapture cap("/home/agustin/facu/tesis/VideosMuestra/test01_reg.wmv"); // open the default camera
    //VideoCapture cap(0);
	if(!cap.isOpened()){
    	// check if we succeeded
    	std::cout << "There are no cammera\n";
    	return -1;
    }

	std::cout << "SetWidth: " << cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280) << std::endl;
	std::cout << "SetHeight " << cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720) << std::endl;
	double h = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	std::cout << "Height: " << h << std::endl;
	h = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	std::cout << "Width: " << h << std::endl;



    Mat edges;
    double duration = 0;
    double elapsedTime = 0;
    int nFrames = 0;
    namedWindow("edges",1);
    for(;;)
    {
    	duration = static_cast<double>(cv::getTickCount());
        Mat frame;

        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 80, 200, 3);

        paintOfGreen(edges);

        imshow("edges", edges);


        duration = static_cast<double>(cv::getTickCount()) - duration;
        duration /= cv::getTickFrequency();
        elapsedTime += duration;
        std::cout << "FrameTime: " << duration << "\n";
        ++nFrames;
        if(elapsedTime >= 1.0){
        	elapsedTime = 0;
        	std::cout << "Numeros frame por segundo: " << nFrames << std::endl;
        	nFrames = 0;
        }

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
