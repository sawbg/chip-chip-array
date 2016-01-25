#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

#include "definitions.hpp"
#include "PiCamera.hpp"

using namespace ChipChipArray;
using namespace cv;
using namespace std;

/**
 *
 */
int main() {
	PiCamera cam;
	string control = "Settings";
	string winThresh = "Image Threshold";
	string winContours = "Contours Detected";

	int lowH = 0;
	int highH = 255;
	int lowS = 0;
	int highS = 255;
	int lowV = 0;
	int highV = 255;
	int polyEps = 3;

	namedWindow(control, CV_WINDOW_AUTOSIZE);
	namedWindow(winThresh, CV_WINDOW_AUTOSIZE);
	namedWindow(winContours, CV_WINDOW_AUTOSIZE);

	createTrackbar("Hue Min", control, &lowH, highH);
	createTrackbar("Hue Max", control, &highH, highH);
	createTrackbar("Sat Min", control, &lowS, highS);
	createTrackbar("Sat Max", control, &highS, highS);
	createTrackbar("Val Min", control, &lowV, highV);
	createTrackbar("Val Max", control, &highV, highV);
	createTrackbar("Polygon Epsilon", control, &polyEps, 20);

	Mat imgTemp = cam.Snap();
	Mat imgLines = Mat::zeros(imgTemp.size(), CV_8UC3);

	while(true) {
		Mat imgOrig = cam.Snap();
		Mat imgHSV;
		Mat imgThresh;
		cvtColor(imgOrig, imgHSV, COLOR_BGR2HSV);
		
		inRange(imgHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS,
					highV), imgThresh);
		erode(imgThresh, imgThresh, getStructuringElement(
					MORPH_ELLIPSE, cv::Size(5, 5)));
		dilate(imgThresh, imgThresh, getStructuringElement(
					MORPH_ELLIPSE, cv::Size(5, 5)));
		dilate(imgThresh, imgThresh, getStructuringElement(
					MORPH_ELLIPSE, cv::Size(5, 5)));
		erode(imgThresh, imgThresh, getStructuringElement(
				       MORPH_ELLIPSE, cv::Size(5, 5)));

		imshow(winThresh, imgThresh);

		vector<vector<Point>> contours;
		findContours(imgThresh, contours, CV_RETR_TREE,
				CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		vector<vector<Point>> contours_poly(contours.size());
		vector<Rect> bounds(contours.size());

		for(int i = 0; i < contours.size(); i++) {
			approxPolyDP(Mat(contours[i]), contours_poly[i],
						polyEps, false);
			bounds[i] = boundingRect(Mat(contours_poly[i]));
		}

		for(int i = 0; i < contours.size(); i++) {
			rectangle(imgOrig, bounds[i].tl(), bounds[i].br(),
					Scalar(255, 0, 0), 2, 8, 0);
		}

		imshow(winContours, imgOrig);
		waitKey(50);
	}
}
