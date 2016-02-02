#ifndef Grabber_H
#define Grabber_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <vector>

#include "definitions.hpp"
#include "Log.hpp"
#include "PiCamera.hpp"

namespace ChipChipArray {

	/**
	 * This class finds blocks, identifies them, and sorts them according to
	 * color, size, and zone.
	 */
	class Grabber {
		public:
			/**
			 * Initializes the class according to the side and zone
			 * and extends the robotic arm into position.
			 *
			 * @param zone the zone (A, B, or C) for which to pick
			 * up blocks.
			 *
			 * @param side the side from which the robot is moving
			 * and the position of the blocks (right or left) in the
			 * view of the camera to pick up first
			 */
			Grabber(Zone zone, Side side);

			/**
			 * Destructs the class and retracts the arm.
			 */
			~Grabber();

			/**
			 * Loads a block(s) (if possible) at the robot's current
			 * position.
			 *
			 * @return the number of half and whole blocks loaded
			 */
			Result Load();

			void Close();

			Block LocateBlock();


		protected:
			/**
			 * Takes block currently in the arm's pinchers and
			 * places to side of robot opposite the loading zone.
			 */
			void Discard();

			/**
			 * Sets arm to generic position roughly right above a
			 * stack of blocks.
			 */
			void Extend();

			/**
			 * Whether there is little doubt that a block is within
			 * reach based on the image from the camera.
			 *
			 * @return if a block is within reach
			 */
			bool FindBlock();

			/**
			 * Examines block in pinchers for a definite color and
			 * returns most likely Color. For Zones B and C only.
			 *
			 * @return the Color of the current block
			 */
			Color FindColor();


			/**
			 * Determines if block is short or long. It is assumed
			 * to be long if there is uncertainty. Zone B only.
			 *
			 * @return the Size of the current block
			 */
			Size FindSize();

			/**
			 * Picks up block by ezaming how much it fills up the
			 * camera image and where its edges are. It is then
			 * decided where to grab the block, and the block is
			 * picked up.
			 *
			 * @param pos whether the (half-)block is closer to the
			 * robot (FRONT) or farther away (BACK).
			 */
			void Grab(BlockPosition pos);

			/**
			 * Puts block in slot according to color. Zones B and C
			 * only.
			 *
			 * @param the slot into which to place the block
			 */
			void Place(Color color);

			/**
			 * Puts half-blocks in the unloader according to whether
			 * there is already a half-block in the corresponding
			 * slot. Zone B only.
			 *
			 * @param color the slot into which to place the block
			 *
			 * @param pos the block position (FRONT/BACK). BACK
			 * denotes there is already a half-block stored in the
			 * slot, as the FRONT blocks are picked up first.
			 */
			void Place(Color color, BlockPosition pos);

		private:
			/**
			 *
			 */
			const uint32 MIN_HALF_BLOCK_SIZE = 200000;

			/**
			 *
			 */
			uint8 invokeCount = 0;

			/**
			 *
			 */
			static Log log;

			/**
			 *
			 */
			std::map<Color, std::array<cv::Scalar, 2>> rangeVals;

			/**
			 *
			 */
			PiCamera cam;
	};

	/**
	 * need this to prevent linker errors
	 */
	Log Grabber::log("logs/Grabber", LogMode::Multi);

	Grabber::Grabber(Zone zone, Side side) {
		log.Verbose("Initializing Grabber");
		log.Verbose("Zone: " + std::to_string(zone));
		log.Verbose("Side: " + std::to_string(side));

		invokeCount++;

		log.Verbose("Setting HSV threshold values");
		rangeVals[Color::Red] = { cv::Scalar(0, 165, 0),
			cv::Scalar(10, 255, 100) };
		rangeVals[Color::Yellow] = { cv::Scalar(25, 224, 114),
			cv::Scalar(36, 225, 183) };
		rangeVals[Color::Green] = { cv::Scalar(44, 128, 0),
			cv::Scalar(70, 255, 124) };
		rangeVals[Color::Blue] = { cv::Scalar(70, 128, 0),
			cv::Scalar(113, 255, 255) };

	}

	Grabber::~Grabber() {

	}

	void Grabber::Close() {
		log.Status("Closing Grabber");
		cv::destroyAllWindows();
		cam.Close();
	}

	void Grabber::Discard() {

	}

	void Grabber::Extend() {

	}

	bool Grabber::FindBlock() {

	}

	Color Grabber::FindColor() {

	}

	Size Grabber::FindSize() {

	}

	void Grabber::Grab(BlockPosition pos) {

	}

	Result Grabber::Load() {

	}

	Block Grabber::LocateBlock() {
		log.Verbose("Locating block");

		cv::Mat imgOrig = cam.Snap();
		cv::Mat imgHSV;
		cv::Mat imgThresh;
		std::vector<cv::Rect> blocks;
		std::vector<Color> colors;

		for(int i = 0; i < rangeVals.size(); i++) {
			// bc c++ can be just weird sometimes
			Color color;

			switch(i) {
				case 0:
					color = Color::Red;
					break;

				case 1:
					color = Color::Yellow;
					break;

				case 2:
					color = Color::Green;
					break;

				case 3:
					color = Color::Blue;
					break;

				default:
					color = Color::Red;  // for funzies
					break;
			}

			log.Status("Searching: " + std::to_string(color));
			cv::cvtColor(imgOrig, imgHSV, cv::COLOR_BGR2HSV);

			cv::Scalar low[2] = rangeVals[color][0];
			cv::Scalar high[2] = rangeVals[color][1];

			cv::inRange(imgHSV, rangeVals[color][0],
					rangeVals[color][1], imgThresh);

			/* 
			 * Not quite sure what all this does, but it seems to
			 * relate to smoothing the image
			 */
			cv::erode(imgThresh, imgThresh,
					cv::getStructuringElement(
						cv::MORPH_ELLIPSE,
						cv::Size(5, 5)));
			cv::dilate(imgThresh, imgThresh,
					cv::getStructuringElement(
						cv::MORPH_ELLIPSE,
						cv::Size(5, 5)));
			cv::dilate(imgThresh, imgThresh,
					cv::getStructuringElement(
						cv::MORPH_ELLIPSE,
						cv::Size(5, 5)));
			cv::erode(imgThresh, imgThresh,
					cv::getStructuringElement(
						cv::MORPH_ELLIPSE,
						cv::Size(5, 5)));

			log.Image(imgThresh, "thresh_" + std::to_string(color)
					+ "_" + std::to_string(invokeCount)
					+ ".bmp");

			// calculate contours
			std::vector<std::vector<cv::Point>> contours;
			cv::findContours(imgThresh, contours, CV_RETR_TREE,
					CV_CHAIN_APPROX_SIMPLE,
					cv::Point(0, 0));
			std::vector<std::vector<cv::Point>>
				contours_poly(contours.size());
			std::vector<cv::Rect> bounds(contours.size());

			// find rectangle around polygon-ish shapes
			for(int i = 0; i < contours.size(); i++) {
				cv::approxPolyDP(cv::Mat(contours[i]),
						contours_poly[i], 20,
						false);
				cv::Rect rect = cv::boundingRect(
						cv::Mat(contours_poly[i]));
				uint32 area = rect.width * rect.height;

				if(area > MIN_HALF_BLOCK_SIZE) {
					log.Debug(std::to_string(color)
							+ " block detected "
							"with area "
							+ std::to_string(
								area));
					blocks.push_back(rect);
					colors.push_back(color);
				}
			}

		}

		if(blocks.size() == 0) {
			throw std::runtime_error("No blocks found!");
		} else {
			log.Status(std::to_string(blocks.size())
					+ " blocks found");
		}

		// coordinates start in top right
		Block block = Block(blocks[0], colors[0]);

		if(blocks.size() > 1) {
			for(int i = 1; i < blocks.size(); i++) { 
				if(blocks[i].x > block.topLeft.x) {
					block = Block(blocks[i], colors[i]);
				}
			}
		}

		log.Status(std::to_string(block.color) + " block is located");

		/* 
		 * Draw surrounding rectangles from above on original
		 * image.
		 */
		cv::rectangle(imgOrig, block.topLeft , block.bottomRight,
				cv::Scalar(255, 0, 0), 4, 8);
		log.Image(imgOrig, "original_" + std::to_string(invokeCount)
				+ ".bmp");

		return block;
	}
}

#endif
