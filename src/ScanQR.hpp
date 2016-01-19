#ifndef ScanQR_H
#define ScanQR_H

#include <ctime>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar>

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
		bool cont = true;
		const uint8 duration = 10;
		std::time_t start = std::times(nullptr);

		Color color;
		cv::Mat canvas;
		PiCamera cam(false);
		zbar::Image image;
		zbar::ImageScanner scanner;

#ifdef DEBUG
		std::string window = "Searching...";

		namedWindow(window, WINDOW_NORMAL);
#endif

		scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

		// 1. Position arm

		// 2. Scan images from camera
		while(std::time(nullptr) - start < duration && cont) {
			canvas = cam.Snap();

#ifdef DEBUG
			imshow(window, canvas);
#endif

			image = new Image(canvas.cols, canvas.rows, "Y800",
					(ubyte*)canvas.data,
					canvas.cols * canvas.rows);

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

				cont = false;


#ifdef DEBUG
				destroyWindow(window);
#endif
			}
		}


		delete *cam;  // must be deleted!
		return color;
	}
}

#endif
