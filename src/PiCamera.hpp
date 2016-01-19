#ifndef PiCamera_H
#define PiCamera_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <raspicam/raspicam_cv.h>

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
			 * Releases Raspicam and other resources.
			 */
			~PiCamera();

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
		cam.set(CV_CAP_PROP_FORMAT, useColor ? CV_16UC3 : CV_16UC1);
		cam.open();
	}

	PiCamera::~PiCamera() {
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
