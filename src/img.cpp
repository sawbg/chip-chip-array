/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief Contains a program to display the current camera image.
 *  */

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
 * This program displays the current camera image. 
 */
int main() {
	string window = "Current";
	PiCamera cam;
	
	namedWindow(window, CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

	while(true) {
		Mat image;
		transpose(cam.Snap(), image);
		imshow(window, image);
		waitKey(10);  // has to be here :(
	}
}
