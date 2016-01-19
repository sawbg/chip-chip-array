#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <iostream>

#include "raspicam_cv.h"

int main() {
	cv::namedWindow("Test", CV_WINDOW_AUTOSIZE);

	raspicam::RaspiCam_Cv cam;
	cv::Mat image;

	bool s = cam.set(CV_CAP_PROP_FORMAT, CV_16UC3);
	bool o = cam.open();

	while(true) {
		cam.grab();
		cam.retrieve(image);
		cv::imshow("Test", image);
		cv::waitKey(1000);

	}

	cam.release();
}
