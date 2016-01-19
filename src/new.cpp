#include "PiCamera.hpp"

using namespace ChipChipArray;
using namespace cv;

int main() {
	namedWindow("Test", CV_WINDOW_AUTOSIZE);
	PiCamera cam(false);

	while(true) {
		imshow("Test", cam.Snap());
		waitKey(20);
	}
	
	delete &cam;
}
