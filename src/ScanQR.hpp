#ifndef ScanQR_H
#define ScanQR_H

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>

#include "definitions.hpp"
#include "PiCamera.hpp"

namespace ChipChipArray {

	/**
	 * This function manuvers arm to look at the QR code on a train car as
	 * the robot is backing up to the car. It attempts to find the code in
	 * multiple images before finally throwing an exeption if a code is not
	 * found. If multiple codes are found, it returns a single Color by
	 * (seemingly) arbitrary decision.
	 *
	 * This function is based on code written by Michael Young
	 * (https://github.com/ayoungprogrammer/WebcamCodeScanner).
	 */
	Color ScanQR() {
		// 0. Initialize variables
		Color color;
		PiCamera cam(false);
#ifdef DEBUG
		std::string window = "Searching...";
		cv::namedWindow(window, CV_WINDOW_AUTOSIZE);
#endif

		// 1. Position arm

		// 2. Scan images from camera
		while(true) {
			cv::Mat frame = cam.Snap();
			cv::Mat canvas;
			cv::cvtColor(frame, canvas, CV_BGR2GRAY); 
#ifdef DEBUG
			cv::imshow(window, canvas);

			 // has to be called right after imshow()
			cv::waitKey(10);
#endif
			uint32 width = canvas.cols;
			uint32 height = canvas.rows;
			zbar::Image image(width, height, "Y800",
					(uchar*)canvas.data, width * height);

			zbar::ImageScanner scanner;
			scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
			scanner.scan(image);
			zbar::Image::SymbolIterator symbol = image.symbol_begin();

			if(symbol != image.symbol_end()) {
				switch(symbol->get_data()[0]) {
					case 'r':
						color = Color::Red;
						break;

					case 'y':
						color = Color::Yellow;
						break;

					case 'g':
						color = Color::Green;
						break;

					case 'b':
						color = Color::Blue;
						break;
				}

				break;

#ifdef DEBUG
				cv::destroyWindow(window);
#endif
			}
		}

		// 3. Retract arm
		
		return color;
	}
}

#endif
