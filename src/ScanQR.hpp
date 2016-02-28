#ifndef ScanQR_H
#define ScanQR_H

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>

#include "definitions.hpp"
#include "Log.hpp"
#include "PiCamera.hpp"

namespace ChipChipArray {

	uint8 qrInvokeCount = 0;
	Log scanQrLog("logs/ScanQR", LogMode::Multi);

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

		// 1. Position arm
		scanQrLog.Verbose("Positioning arm");

		// 2. Scan images from camera
		scanQrLog.Verbose("Scanning for QR code");

		// Nick's supposed to make sure this isn't an endles loop
		while(true) {			
			// get image
			cv::Mat frame = cam.Snap();
			cv::Mat canvas;
			cv::cvtColor(frame, canvas, CV_BGR2GRAY);
			scanQrLog.Image(canvas, std::to_string(++qrInvokeCount)
					+ ".bmp");
			
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

				scanQrLog.Status("Detected " + std::to_string(color)
						+ " train car");
				break;
			}
		}

		// 3. Retract arm
		scanQrLog.Verbose("Retracting arm");		
		return color;
	}
}

#endif
