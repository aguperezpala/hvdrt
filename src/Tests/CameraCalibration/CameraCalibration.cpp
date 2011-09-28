/*
 * CameraCalibration.cpp
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
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <cstdlib>
#include <vector>

typedef struct {
	int r;
	int g;
	int b;
} RGB_t;

#define BOUNDARY_COLOR	60

static void ownFilter(cv::Mat &image, const RGB_t &color)
{
	cv::Scalar min = CV_RGB(color.r - BOUNDARY_COLOR/2,
			color.g - BOUNDARY_COLOR/2,
			color.b - BOUNDARY_COLOR/2);
	cv::Scalar max = CV_RGB(color.r + BOUNDARY_COLOR/2,
				color.g + BOUNDARY_COLOR/2,
				color.b + BOUNDARY_COLOR/2);

	cv::inRange(image, min, max, image);

}


class CameraCalibrator {
	// input points:
	// the points in world coordinates
	std::vector<std::vector<cv::Point3f> > objectPoints;
	// the point positions in pixels
	std::vector<std::vector<cv::Point2f> > imagePoints;
	// output Matrices
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	// flag to specify how calibration is done
	int flag;

	// used in image undistortion
	cv::Mat map1,map2;
	bool mustInitUndistort;

public:
	std::vector<cv::Point2f> imageCorners;
public:
	CameraCalibrator() : flag(0), mustInitUndistort(true) {};
	// Open chessboard images and extract corner points
	int addChessboardPoints(
			const std::vector<std::string>& filelist,
			cv::Size & boardSize) {
		// the points on the chessboard
		std::vector<cv::Point3f> objectCorners;
		// 3D Scene Points:
		// Initialize the chessboard corners
		// in the chessboard reference frame
		// The corners are at 3D location (X,Y,Z)= (i,j,0)
		for (int i=0; i<boardSize.height; i++) {
			for (int j=0; j<boardSize.width; j++) {
				objectCorners.push_back(cv::Point3f(i*10, j*100, 0.0f));
			}
		}
		// 2D Image points:
		cv::Mat image; // to contain chessboard image
		int successes = 0;
		// for all viewpoints
		for (int i=0; i<filelist.size(); i++) {
			// Open the image
			image = cv::imread(filelist[i],0);
			// Get the chessboard corners
			bool found = cv::findChessboardCorners(
					image, boardSize, imageCorners);
			// Get subpixel accuracy on the corners
			cv::cornerSubPix(image, imageCorners,
					cv::Size(5,5),
					cv::Size(-1,-1),
					cv::TermCriteria(cv::TermCriteria::MAX_ITER +
							cv::TermCriteria::EPS,
							30,
							// max number of iterations
							0.1)); // min accuracy
			//If we have a good board, add it to our data
			if (imageCorners.size() == boardSize.area()) {
				// Add image and scene points from one view
				addPoints(imageCorners, objectCorners);
				successes++;
			}
		}
		return successes;
	}


	// Add scene points and corresponding image points
	void addPoints(const std::vector<cv::Point2f>&
			imageCorners, const std::vector<cv::Point3f>& objectCorners) {
		// 2D image points from one view
		imagePoints.push_back(imageCorners);
		// corresponding 3D scene points
		objectPoints.push_back(objectCorners);
	}
	// Calibrate the camera
	// returns the re-projection error
	double calibrate(cv::Size &imageSize)
	{
		// undistorter must be reinitialized
		mustInitUndistort= true;
		//Output rotations and translations
		std::vector<cv::Mat> rvecs, tvecs;
		// start calibration
		return
				calibrateCamera(objectPoints, // the 3D points
						imagePoints,
						imageSize,
						cameraMatrix,
						distCoeffs,
						rvecs, tvecs,
						flag);
	}

	// remove distortion in an image (after calibration)
	cv::Mat remap(const cv::Mat &image) {
		cv::Mat undistorted;
		if (mustInitUndistort) { // called once per calibration
			cv::initUndistortRectifyMap(
					cameraMatrix, // computed camera matrix
					distCoeffs,
					// computed distortion matrix
					cv::Mat(),
					// optional rectification (none)
					cv::Mat(),
					// camera matrix to generate undistorted
					image.size(), // size of undistorted
					CV_32FC1,
					// type of output map
					map1, map2);
			// the x and y mapping functions
			mustInitUndistort= false;
		}
		// Apply mapping functions
		cv::remap(image, undistorted, map1, map2,
				cv::INTER_LINEAR); // interpolation type
		return undistorted;
	}
};





static void ownRectifier(cv::Mat &img)
{
	/*// hardcoded the CameraMatrix
	cv::Mat result = img.clone();
	cv::Vec<float, 3> l1(317,27,0),
			l2(317,-60,0),
			m1(-17,-312,0),
			m2(-21,-396);

	std::cout << "l1: " << l1[0] << " " << l1[1] << " " << l1[2] << std::endl;
	std::cout << "l2: " << l2[0] << " " << l2[1] << " " << l2[2] << std::endl;
	std::cout << "m1: " << m1[0] << " " << m1[1] << " " << m1[2] << std::endl;
	std::cout << "m2: " << m2[0] << " " << m2[1] << " " << m2[2] << std::endl;

	cv::Vec<float, 3> p1,p2;
	p1 = l1.cross(l2);
	p2 = m1.cross(m2);

	cv::Vec<float, 3> vanishingLine = p1.cross(p2);
	std::cout << "p1: " << p1[0] << " " << p1[1] << " " << p1[2] << std::endl;

	cv::Mat transformMatrix(3,3,CV_64F);
	transformMatrix.at<float>(0,0) = 1;
	transformMatrix.at<float>(0,1) = 0;
	transformMatrix.at<float>(0,2) = 0;

	transformMatrix.at<float>(1,0) = 0;
	transformMatrix.at<float>(1,1) = 1;
	transformMatrix.at<float>(1,2) = 0;

	transformMatrix.at<float>(2,0) = vanishingLine[0];
	transformMatrix.at<float>(2,1) = vanishingLine[1];
	transformMatrix.at<float>(2,2) = vanishingLine[2];


	std::cout << "VAnishing line: " << vanishingLine[0] << "\t" << vanishingLine[1] <<
			"\t" << vanishingLine[2] << std::endl;
			cv::namedWindow("before", 0);
	cv::imshow("before", img);


	std::vector<cv::Point2f> objectPoints;
	objectPoints.push_back(cv::Point2f(0,100));
	objectPoints.push_back(cv::Point2f(100,100));
	objectPoints.push_back(cv::Point2f(0,0));
	objectPoints.push_back(cv::Point2f(100,0));

	std::vector<cv::Point2f> imagePoints;
	imagePoints.push_back(cv::Point2f(165,83));
	imagePoints.push_back(cv::Point2f(478,74));

	imagePoints.push_back(cv::Point2f(181,387));
	imagePoints.push_back(cv::Point2f(500,416));

	cv::Mat cof;
	cv::Mat tm = cv::findHomography(cv::Mat(objectPoints), cv::Mat(imagePoints),CV_RANSAC);
	//cv::undistort(img,result,transformMatrix,cof);
	cv::undistort(img,result,tm, cof);

	cv::namedWindow("after", 0);
	cv::imshow("after", result);

	cv::waitKey();

/*
	// Elegimos nosotros los puntos manualmente
	std::vector<std::vector<cv::Point3f> > v1;
	// the point positions in pixels
	std::vector<std::vector<cv::Point2f> > v2;

	cv::Point3f p1(0,0,0),
			p2(100,0,0),
			l1(0,100,0),
			l2(100,100,0);

	std::vector<cv::Point3f> objectPoints;
	objectPoints.push_back(p1);
	objectPoints.push_back(p2);
	objectPoints.push_back(l1);
	objectPoints.push_back(l2);
	v1.push_back(objectPoints);

	std::vector<cv::Point2f> imagePoints;
	imagePoints.push_back(cv::Point2f(167,82));
	imagePoints.push_back(cv::Point2f(482,22));

	imagePoints.push_back(cv::Point2f(184,389));
	imagePoints.push_back(cv::Point2f(500,418));

	v2.push_back(imagePoints);

	CameraCalibrator cc;
	cc.addPoints(imagePoints,objectPoints);

	cv::Size c = img.size();
	cc.calibrate(c);

	cv::namedWindow("before",0);
	cv::imshow("before", img);

	cc.remap(img);

	cv::namedWindow("after",0);
	cv::imshow("after", img);

	cv::waitKey();

	/*cv::Size c = img.size();
	cv::Mat camMat;
	cv::Mat distCoeffs, o1, o2, o3;
	//cv::calibrateCamera(v1,v2,c,camMat,distCoeffs,o1,o2,0);
	camMat = cv::initCameraMatrix2D(v1,v2,c);

	cv::namedWindow("before",0);
	cv::imshow("before", img);

	cv::undistort(img,o1,camMat,distCoeffs,camMat);

	cv::namedWindow("after",0);
	cv::imshow("after", o1);

	cv::waitKey();
*/


/*

	*/
}


static void usingCameraCalibrator()
{
	cv::Mat img = cv::imread("../src/Tests/CameraCalibration/cheesboard.jpg");
	CameraCalibrator cc;
	std::vector<std::string> f;

	f.push_back("../src/Tests/CameraCalibration/cheesboard.jpg");
	cv::Size boardSize(7,7);
	if(cc.addChessboardPoints(f,boardSize) == 0){
		std::cout << "Error on addChessboardPoints\n";
		return;
	}

	cv::Size s = img.size();
	std::cout << "calibratE: " << cc.calibrate(s) << std::endl;
	bool found;
	cv::drawChessboardCorners(img, boardSize, cc.imageCorners,
										found); // corners have been found

	std::vector<cv::Point2f> objectPoints;
	for (int i=0; i<boardSize.height; i++) {
		for (int j=0; j<boardSize.width; j++) {
			objectPoints.push_back(cv::Point2f(i, j));
		}
	}

	cv::Mat cof;
	cv::Mat tm = cv::findHomography(cv::Mat(cc.imageCorners),cv::Mat(objectPoints),CV_RANSAC);

	cv::namedWindow("before", 0);
	cv::imshow("before", img);

	cv::Mat m = img.clone();
	cv::warpPerspective(img,m,tm, img.size());

//	m = cc.remap(img);
	cv::namedWindow("after", 0);
	cv::imshow("after", m);



	cv::waitKey();
}

static void perspective(void)
{

	// 254
	cv::Mat img = cv::imread("../src/Tests/CameraCalibration/cheesboard.jpg");
	cv::Point2f	c1[4];
	cv::Point2f	c2[4];

	c1[0].x = 253;	c1[0].y = 288;
	c1[1].x = 466;	c1[1].y = 152;
	c1[2].x = 31;	c1[2].y = 158;
	c1[3].x = 247;	c1[3].y = 67;


	c2[2].x = 0;	c2[2].y = 0;
	c2[3].x = 254;	c2[3].y = 0;
	c2[0].x = 0;	c2[0].y = 254;
	c2[1].x = 254;	c2[1].y = 254;


	cv::Mat tm = cv::getPerspectiveTransform(c1,c2);

	cv::namedWindow("before", 0);
	cv::imshow("before", img);

	cv::Mat m = img.clone();
	cv::warpPerspective(img,m,tm, img.size());

//	m = cc.remap(img);
	cv::namedWindow("after", 0);
	cv::imshow("after", m);



	cv::waitKey();
}


static void sample(void)
{
	using namespace cv;
	using namespace std;
	Point2f srcTri[3];
	Point2f dstTri[3];

	Mat rot_mat( 2, 3, CV_32FC1 );
	Mat warp_mat( 2, 3, CV_32FC1 );
	Mat src, warp_dst, warp_rotate_dst;

	/// Load the image
	src = imread("../src/Tests/CameraCalibration/Test.png", 1 );

	/// Set the dst image the same type and size as src
	warp_dst = Mat::zeros( src.rows, src.cols, src.type() );

	/// Set your 3 points to calculate the  Affine Transform
	srcTri[0] = Point2f( 0,0 );
	srcTri[1] = Point2f( src.cols - 1, 0 );
	srcTri[2] = Point2f( 0, src.rows - 1 );

	dstTri[0] = Point2f( src.cols*0.0, src.rows*0.33 );
	dstTri[1] = Point2f( src.cols*0.85, src.rows*0.25 );
	dstTri[2] = Point2f( src.cols*0.15, src.rows*0.7 );

	/// Get the Affine Transform
	warp_mat = getAffineTransform( srcTri, dstTri );

	/// Apply the Affine Transform just found to the src image
	warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

	/** Rotating the image after Warp */

	/// Compute a rotation matrix with respect to the center of the image
	Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
	double angle = -50.0;
	double scale = 0.6;

	/// Get the rotation matrix with the specifications above
	rot_mat = getRotationMatrix2D( center, angle, scale );

	/// Rotate the warped image
	warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );

	/// Show what you got
	namedWindow( "source_window", CV_WINDOW_AUTOSIZE );
	imshow( "source_window", src );

	namedWindow( "warp_window", CV_WINDOW_AUTOSIZE );
	imshow( "warp_window", warp_dst );

	namedWindow( "warp_rotate_window", CV_WINDOW_AUTOSIZE );
	imshow( "warp_rotate_window", warp_rotate_dst );

	/// Wait until user exits the program
	waitKey(0);

}


int main(int argc, char**args)
{

	perspective();
	return 0;

	cv::Mat img;
	img = cv::imread("../src/Tests/CameraCalibration/Distortion.png");

	//ownRectifier(img);

	//return 0;

	// output vectors of image points
	std::vector<cv::Point2f> imageCorners;
	// number of corners on the chessboard
	cv::Size boardSize(7,7);
	cv::Mat image;
	image = cv::imread("../src/Tests/CameraCalibration/cheesboard.jpg");
	// Get the chessboard corners
	bool found = cv::findChessboardCorners(image,
											boardSize, imageCorners);

	if(!found){
		std::cout << "Corners not found\n";
		return 1;
	}
	//Draw the corners
	cv::drawChessboardCorners(image,
									boardSize, imageCorners,
									found); // corners have been found



	cv::namedWindow("win",0);
	cv::namedWindow("own",0);
	cv::imshow("win", image);
	RGB_t color = {225,0,0};
	ownFilter(image, color);
	cv::imshow("own", image);

	cv::waitKey();

	std::cout << "finishing\n";
    return 0;
}
