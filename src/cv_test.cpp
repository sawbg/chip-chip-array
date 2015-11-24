#include <highgui.h>
#include <iostream>

int main() {
	cv::VideoCapture capture(0);
	std::cout << capture.isOpened() << std::endl;
	cv::Mat image;
	capture.read(image);
	cv::imshow("Webcam", image);
}
