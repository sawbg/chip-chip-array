/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief Contains PiCamera class.
 */

#ifndef PiCamera_H
#define PiCamera_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <raspicam/raspicam_cv.h>
#include <unistd.h>

#include "definitions.hpp"

namespace ChipChipArray {

	/**
	 * This class is a basic wrapper to allow the Raspicam to interface with
	 * OpenCV. It uses another wrapper class, Raspicam, provided by Cédric
	 * Verstraeten (https://github.com/cedricve/raspicam).
	 */
	class PiCamera {
		public:
			/**
			 * Opens the camera and configures it for color images.
			 */
			PiCamera() : PiCamera(true) {};
			
			/**
			 * Opens the camera.
			 *
			 * @param useColor Specifices whether camera should make
			 * color images. TRUE = color, FALSE = grayscale.
			 */
			PiCamera(bool useColor);
			
			/**
			 * Closes connection to camera.
			 */
			void Close();

			/**
			 * Makes picture.
			 *
			 * @return OpenCV Mat object (i.e., an image) from the
			 * camera
			 */
			cv::Mat Snap();

		private:
			/**
			 * the Raspicam object provided by raspicam_cv.h
			 */
			raspicam::RaspiCam_Cv cam;
	};

	PiCamera::PiCamera(bool useColor) {
		cam.set(CV_CAP_PROP_FORMAT, (useColor ? CV_16UC3 : CV_16UC1));
		cam.open();
		usleep(500000);  // required to allow camera time to adjust!
	}

	void PiCamera::Close() {
		cam.release();
	}

	cv::Mat PiCamera::Snap() {
		if(!cam.isOpened()) throw std::runtime_error("Camera "
				"is not open!");

		cv::Mat image;
		cam.grab();
		cam.retrieve(image);
		return image;
	}
}

#endif
