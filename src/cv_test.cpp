#include "PiCamera.hpp"

using namespace ChipChipArray;
using namespace cv;

int main() {
	namedWindow("Test", CV_WINDOW_AUTOSIZE);
	PiCamera cam;

	while(true) {
		imshow("Test", cam.Snap());
		waitKey(500);
	}
	
	delete &cam;
}
