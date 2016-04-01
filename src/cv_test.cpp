#include "PiCamera.hpp"

using namespace ChipChipArray;
using namespace cv;

/**
 * This program was used solely to test the PiCamera wrapper class and its
 * compatibility with the raspicam wrapper and ultimately OpenCV.
 */
int main() {
	namedWindow("Test", CV_WINDOW_AUTOSIZE);
	PiCamera cam;

	while(true) {
		imshow("Test", cam.Snap());
		waitKey(500);
	}
	
	delete &cam;
}
