/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief Contains a program that tests Jacob's yellow-detection algorithm.
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "PiCamera.hpp"

using namespace std;
using namespace cv;
using namespace ChipChipArray;

/**
 * This program tests Jacob Laurel's algorithm for detecting yellow blocks
 * (RGB=>YUV, HSV=>RGB). 
 */
int main() {
	PiCamera cam;
	namedWindow("window", CV_WINDOW_NORMAL);
	Mat orig = cam.Snap();
	Mat yuv;
	Mat fin;
	cvtColor(orig, yuv, CV_BGR2YUV);
	cvtColor(yuv, fin, CV_HSV2BGR);
	imshow("window", fin);
	waitKey(-1);
}
