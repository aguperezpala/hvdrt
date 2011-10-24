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

#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>

#include "CoordsInterpreter.h"


using namespace cv;


static void paintPixel(Mat &img, int c, int r)
{
	if(r > 720){
		debug("r:%d\n", r);
		ASSERT(false);
	}
	uchar *data = img.data + r*img.step + c*img.elemSize();
	*data = 200;

}

static void drawToImage(CoordsInterpreter::Data *d, Mat &img)
{
	int nl = img.rows; // number of lines
	// total number of elements per line
	int nc = img.cols * img.channels();
	for (int j = 0; j < nl; j++) {
		// get the address of row j
		uchar* data = img.ptr<uchar> (j);
		for (int i = 0; i < nc; i++) {
			// process each pixel ---------------------
			data[i] = 0;
			// end of pixel processing ----------------
		}
	}


	for(int i = d->size() - 1; i >= 0; --i){
		 std::vector<int> &v = d->at(i);
		for(int j = v.size() -1; j >= 0; --j){
			int p = v[j];
			if(p > nl){
				int s = v.size();
				debug("ERROR: [%d][%d]: %d\n",i,j,p);
				p = 477;
			}
			paintPixel(img, i, p);
		}

	}
}


int main(int, char**)
{
    VideoCapture cap("/home/agustin/facu/tesis/muestreo_videos_lab/test01_reg.avi"); // open the default camera
    if(!cap.isOpened()){
    	// check if we succeeded
    	std::cout << "There are no cammera\n";
    	return -1;
    }

    CoordsInterpreter ci;

    Mat edges;
    Mat frame;


    cap >> frame;
    cvtColor(frame, edges, CV_BGR2GRAY);
	GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
	Canny(edges, edges, 80, 200, 3);

	// create the zone that we want to analize
	cv::Point p1(0,0);
	cv::Point p2(edges.cols, edges.rows);

	cv::Rect r(p1,p2);

	ci.setAnalizeZone(r);
	CoordsInterpreter::Data d;
	ci.setData(&d);

	Mat aux(edges.rows, edges.cols, edges.type());


	ci.processData(edges);


	drawToImage(&d,aux);

	namedWindow("edges");
	imshow("edges", edges);

	namedWindow("CoordsInterpreter");
	imshow("CoordsInterpreter", aux);

	waitKey();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
