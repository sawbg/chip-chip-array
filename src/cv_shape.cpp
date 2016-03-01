/**
 * @author Shermal Fernando
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief contains a program to aid in determining HSV ranges
 */

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

#include "definitions.hpp"
#include "PiCamera.hpp"

using namespace ChipChipArray;
using namespace cv;
using namespace std;

/**
 * This program (a single function) is a test of the computer vision
 * algorithms for loading the blocks. It will likely be in development
 * for some time to come. The plan currently is to develop and test all
 * CV algorithms for block loading here before moving it all into class
 * functions and testing again.
 *
 * This code is based on several online articles:
 * 	- "Color Detectionn & Object Tracking" by Shermal Fernando
 * 	(http://opencv-srf.blogspot.com/2010/09/object-detection-using-color-seperation.html)
 * 	- "Shape Detection & Tracking using Contours" by Shermal
 * 	Fernando
 * 	(http://opencv-srf.blogspot.com/2011/09/object-detection-tracking-using-contours.html)
 * 	- "Creating Bounding boxes and circles for contours" in the
 * 	OpenCV 2.4 Tutorials
 * 	(http://opencv-srf.blogspot.com/2011/09/object-detection-tracking-using-contours.html)
 */
int main() {
	PiCamera cam;

	// window names
	string control = "Settings";
	string winThresh = "Image Threshold";
	string winContours = "Contours Detected";

	// control (trackbar) variables
	int lowH = 0;  // hue
	int highH = 255;
	int lowS = 0;  // saturation
	int highS = 255;
	int lowV = 0;  // value
	int highV = 255;
	int polyEps = 3;  // max dif b/t bin shape edge & est poly edge

	// opening windows
	namedWindow(control, CV_WINDOW_AUTOSIZE);
	namedWindow(winThresh, CV_WINDOW_AUTOSIZE);
	namedWindow(winContours, CV_WINDOW_AUTOSIZE);

	// creating control trackbars
	createTrackbar("Hue Min", control, &lowH, highH);
	createTrackbar("Hue Max", control, &highH, highH);
	createTrackbar("Sat Min", control, &lowS, highS);
	createTrackbar("Sat Max", control, &highS, highS);
	createTrackbar("Val Min", control, &lowV, highV);
	createTrackbar("Val Max", control, &highV, highV);
	createTrackbar("Polygon Epsilon", control, &polyEps, 20);

	while(true) {
		Mat imgOrig = cam.Snap();  // real iage
		Mat imgHSV;  // RGB image converted to HSV space
		Mat imgThresh;  // binary threshold image
		cvtColor(imgOrig, imgHSV, CV_BGR2YUV);
		cvtColor(imgHSV, imgOrig, CV_HSV2BGR);
		cvtColor(imgOrig, imgHSV, COLOR_BGR2HSV);

		// create binary image
		inRange(imgHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS,
					highV), imgThresh);

		/* 
		 * Not quite sure what all this does, but it seems to
		 * relate to smoothing the image
		 */
		erode(imgThresh, imgThresh, getStructuringElement(
					MORPH_ELLIPSE, cv::Size(5, 5)));
		dilate(imgThresh, imgThresh, getStructuringElement(
					MORPH_ELLIPSE, cv::Size(5, 5)));
		dilate(imgThresh, imgThresh, getStructuringElement(
					MORPH_ELLIPSE, cv::Size(5, 5)));
		erode(imgThresh, imgThresh, getStructuringElement(
					MORPH_ELLIPSE, cv::Size(5, 5)));

		// show binary image in threshold window
		imshow(winThresh, imgThresh);

		// calculate contours
		vector<vector<Point>> contours;
		findContours(imgThresh, contours, CV_RETR_TREE,
				CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		vector<vector<Point>> contours_poly(contours.size());
		vector<Rect> bounds(contours.size());
		int maxArea = 0;
		int offset;

		// find rectangle around polygon-ish shapes
		for(int i = 0; i < contours.size(); i++) {
			approxPolyDP(Mat(contours[i]), contours_poly[i],
					polyEps, false);
			bounds[i] = boundingRect(Mat(contours_poly[i]));
		}

		/* 
		 * Draw surrounding rectangles from above on original
		 * image.
		 */
		for(int i = 0; i < contours.size(); i++) {
			rectangle(imgOrig, bounds[i].tl(), bounds[i].br(),
					Scalar(255, 0, 0), 2, 8, 0);
			//drawContours(imgOrig, contours_poly, i,
			//		Scalar(255, 0, 0), 4, 8);
			int area = bounds[i].width * bounds[i].height;

			if(area > maxArea) {
				offset = abs(640 - (bounds[i].tl().x + bounds[i].width / 2));
				maxArea = area;
			}
		}

		cout << "Block area: " << maxArea << " pixels\t\t" 
			<< "Center offset: " << offset << endl;
		imshow(winContours, imgOrig);  // show original image with rectangles
		waitKey(50);  // has to be here :(
	}
}
