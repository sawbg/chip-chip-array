/**
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief contains the Grabber class
 */

#ifndef Grabber_H
#define Grabber_H

#include <cmath>
#include <map>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <unistd.h>
#include <vector>

#include "Arm.hpp"
#include "definitions.hpp"
#include "Block.hpp"
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
			 * Closes the Grabber. Retracts the arm and closes the
			 * camera.
			 */
			void Close();


			/**
			 * Loads a block(s) (if possible) at the robot's current
			 * position.
			 *
			 * @return the number of half and whole blocks loaded
			 */
			Result Load();

		protected:
			/**
			 * The Raspicam
			 */
			PiCamera cam;

			/**
			 * The side from which the robot is coming (i.e., the
			 * side where the higher priority blocks are to be
			 * picked up.
			 */
			Side side;

			/**
			 * The boat zone
			 */
			Zone zone;

			/**
			 *
			 */
			void Deposit(Color color = Color::Blue);

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
			 * Picks up block.
			 *
			 * @param layer whether the block is on the top of the stack or the
			 * bottom
			 *
			 * @param pos whether the (half-)block is closer to the
			 * robot (FRONT) or farther away (BACK). MIDDLE indicates a whole
			 * block.
			 */
			void Grab(Layer layer, BlockPosition pos = BlockPosition::Middle);

			/**
			 *
			 */
			Block LocateBlocks(Color color = Color::Perrywinkle);

			/**
			 *
			 */
			Block LocateBlueBlock();

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
			 * The minimum area in pixels to be considered a block
			 */
			static const uint32 MIN_HALF_BLOCK_SIZE = 50000;

			/**
			 * The log object to be shared among the Grabber
			 * objects
			 */
			static Log log;

			/**
			 * The number of times the method has been called in
			 * this instance of the Grabber.
			 */
			uint8 invokeCount = 0;

			/**
			 * Contains the HSV threshold values for each color of
			 * block
			 */
			std::map<Color, std::array<cv::Scalar, 2>> rangeVals;

			/**
			 * The robotic arm class interface.
			 */
			Arm arm;
	};

	/**
	 * need this to prevent linker errors
	 */
	Log Grabber::log("logs/Grabber", LogMode::Multi);

	Grabber::Grabber(Zone zone, Side side) {
		log.Status("Opening Grabber");
		log.Verbose("Zone: " + std::to_string(zone));
		log.Verbose("Side: " + std::to_string(side));

		this->zone = zone;
		this->side = side;

		log.Verbose("Setting HSV threshold values");

		rangeVals[Color::Red] = { cv::Scalar(0, 20, 60),
			cv::Scalar(12, 255, 255) };
		rangeVals[Color::Green] = { cv::Scalar(49, 41, 17),
			cv::Scalar(63, 255, 255) };
		rangeVals[Color::Blue] = { cv::Scalar(70, 0, 0),
			cv::Scalar(100, 255, 255) };

		/* Remember, we're only pretending this color's image is in HSV space.
		 * It's really in YUV, as required by Jacob yellow-detection algorithm. */
		rangeVals[Color::Yellow] = { cv::Scalar(0, 0, 0),
			cv::Scalar(255, 255, 20)};
	}

	void Grabber::Close() {
		log.Status("Closing Grabber");
		cam.Close();
	}

	void Grabber::Deposit(Color color) {
		if(color == Color::Blue) {
			arm.ClawClose();
			sleep(1);
			arm.BaseTilt(160);
			sleep(1);
			arm.Elbow(130);
			sleep(1);
			arm.BaseTurn(47);
			sleep(1);
			arm.ClawOpen();
			sleep(1);
		} else {
			throw std::runtime_error("Du Idiot! Die Armbewegungen für diese "
					"Farbe sind noch nicht implementiert. Vielleicht sollst "
					"du die englische Phrase lernen 'Would you like fries "
					"with that?");
		}
	}

	void Grabber::Extend() {
		arm.Elbow(180);
		usleep(500000);
		arm.BaseTurn(132);
		arm.BaseTilt(125);
		arm.Elbow(150);
		arm.WristTwist(90);
		arm.ClawOpen();
		sleep(2);
	}

	void Grabber::Grab(Layer layer, BlockPosition pos) {

	}

	Result Grabber::Load() {
		for(uint8 i = 0; i < 2; i++) {
			Extend();

			try {
				Block block = (zone == Zone::A)
					? LocateBlocks(Color::Blue) : LocateBlueBlock();

				float32 baseKonstant = 0.5;
				if(block.dRightLeft > 0) baseKonstant *= -1;
				float32 degree = baseKonstant * std::sqrt(block.dRightLeft);
				arm.dBaseTurn(degree);
				arm.dWristTwist(-degree);
				sleep(1);
				arm.BaseTilt(140);
				sleep(1);

				uint8 bend = (i == 0 ? 100 : 90);

				// lower claw over block
				for(uint8 j = 140; j >= bend; j -= 10) {
					arm.Elbow(j);
					sleep(1);
				}

				// deposit in bin
				sleep(1);
				Deposit();
			} catch(std::exception ex) {
				log.Error(std::string("An exception occured attempting "
							"to load the blocks in function Grabber::Load(): ") 
						+ ex.what());
			}


			if(i == 0) {
				arm.BaseTurn(132);
			} else {
				arm.BaseTurn(135);
				sleep(1);
				arm.BaseTilt(180);
				sleep(1);
				arm.Elbow(90);
				sleep(1);
				arm.Elbow(45);
				sleep(1);
				arm.Elbow(0);
			}
		}
	}


	Block Grabber::LocateBlocks(Color color) {
		invokeCount++;
		std::string logstr = "Locating blocks";

		if(color == Color::Perrywinkle) {
			logstr += " (" + std::to_string(color) + ")";
		}

		log.Verbose(logstr);

		cv::Mat imgOrig;
		cv::transpose(cam.Snap(), imgOrig);

		cv::Mat imgHSV;
		cv::Mat imgThresh;
		std::vector<cv::Rect> blocks;
		std::vector<Color> colors;

		uint8 loopNum = (color == Color::Perrywinkle ? rangeVals.size() : 1);

		for(int i = 0; i < loopNum;	i++) {
			if(loopNum > 1) {
				switch(i) {
					case 0:
						color = Color::Red;
						break;

					case 1:
						color = Color::Green;
						break;

					case 2:
						color = Color::Blue;
						break;

						/* Must be last, because it changes imgHSV from HSV space
						 * to YUV space. */
					case 3: 
						color = Color::Yellow;
						break;
				}
			}

			log.Verbose("Searching: " + std::to_string(color));

			if(color == Color::Yellow) {
				cv::Mat temp;
				imgOrig.copyTo(temp);
				cv::cvtColor(imgOrig, imgHSV, CV_BGR2YUV);
				cv::cvtColor(imgHSV, temp, CV_HSV2BGR);
				cv::cvtColor(temp, imgHSV, cv::COLOR_BGR2HSV);
				log.Image(temp, "yuv_yellow_" + std::to_string(color)
						+ "_" + std::to_string(zone)
						+ std::to_string(invokeCount)
						+ ".bmp");
			} else {
				cv::cvtColor(imgOrig, imgHSV, cv::COLOR_BGR2HSV);
			}

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
					+ "_" + std::to_string(zone)
					+ std::to_string(invokeCount)
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
				uint32 area = cv::contourArea(contours[i]);

				// determine if block and add to blocks vector
				if(area > MIN_HALF_BLOCK_SIZE) {
					cv::approxPolyDP(cv::Mat(contours[i]),
							contours_poly[i], 20,
							false);
					cv::Rect rect = cv::boundingRect(
							cv::Mat(contours_poly[i]));
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
			log.Image(imgOrig, "original_" + std::to_string(zone)
					+ std::to_string(invokeCount)
					+ "_no_blocks.bmp");
			throw std::runtime_error("No blocks found!");
		} else {
			log.Status(std::to_string(blocks.size())
					+ " blocks found");
		}

		// coordinates start in top right
		Block block = Block(blocks[0], colors[0]);

		if(blocks.size() > 1) {
			for(int i = 1; i < blocks.size(); i++) { 
				if((side == Side::Right && blocks[i].x 
							> block.topLeft.x)
						|| (side == Side::Left
							&& blocks[i].x
							< block.topLeft.x)) {
					block = Block(blocks[i], colors[i]);
				}
			}
		}

		log.Status(std::to_string(block.color) + " block is located");

		log.Debug("Block properties => area: " + std::to_string(block.area)
				+ ", height: " + std::to_string(block.height) + ", width: "
				+ std::to_string(block.width) + ", offset: "
				+ std::to_string(block.offset) + ", color: "
				+ std::to_string(block.color) + ", size: "
				+ std::to_string(block.size));

		/* 
		 * Draw surrounding rectangles from above on original
		 * image.
		 */
		cv::rectangle(imgOrig, block.topLeft , block.bottomRight,
				cv::Scalar(255, 0, 0), 4, 8);
		log.Image(imgOrig, "original_" + std::to_string(zone)
				+ std::to_string(invokeCount)
				+ ".bmp");

		return block;
	}

	Block Grabber::LocateBlueBlock() {
		std::vector<cv::Mat> channels;
		std::vector<cv::Rect> blocks;	

		invokeCount++;
		log.Status("Locating blue blocks");

		cv::Mat img;
		cv::Mat imgThresh;
		cv::split(cam.Snap(), channels);
		cv::transpose(channels[0], img);

		log.Verbose("Searching: Blue block");
		cv::inRange(img, 30, 255, imgThresh);
		log.Image(imgThresh, "thresh_blue" + std::to_string(zone)
				+ std::to_string(invokeCount) + ".bmp");

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
			uint32 area = cv::contourArea(contours[i]);

			// determine if block and add to blocks vector
			if(area > MIN_HALF_BLOCK_SIZE) {
				cv::approxPolyDP(cv::Mat(contours[i]),
						contours_poly[i], 20,
						false);
				cv::Rect rect = cv::boundingRect(
						cv::Mat(contours_poly[i]));
				log.Debug("Blue block detected with area "
						+ std::to_string(area));
				blocks.push_back(rect);
			}
		}


		if(blocks.size() == 0) {
			log.Image(img, "original_" + std::to_string(zone)
					+ std::to_string(invokeCount)
					+ "_no_blocks.bmp");
			throw std::runtime_error("No blocks found!");
		} else {
			log.Status(std::to_string(blocks.size())
					+ " blocks found");
		}

		// coordinates start in top right
		Block block = Block(blocks[0], Color::Blue);

		if(blocks.size() > 1) {
			for(int i = 1; i < blocks.size(); i++) { 
				if((side == Side::Right && blocks[i].x 
							> block.topLeft.x)
						|| (side == Side::Left
							&& blocks[i].x
							< block.topLeft.x)) {
					block = Block(blocks[i], Color::Blue);
				}
			}
		}

		log.Status(std::to_string(block.color) + " block is located");

		log.Debug("Block properties => area: " + std::to_string(block.area)
				+ ", height: " + std::to_string(block.height) + ", width: "
				+ std::to_string(block.width) + ", offset: "
				+ std::to_string(block.offset) + ", color: "
				+ std::to_string(block.color) + ", size: "
				+ std::to_string(block.size));

		/* 
		 * Draw surrounding rectangles from above on original
		 * image.
		 */
		cv::rectangle(img, block.topLeft , block.bottomRight,
				cv::Scalar(255, 0, 0), 4, 8);
		log.Image(img, "original_" + std::to_string(zone)
				+ std::to_string(invokeCount)
				+ ".bmp");

		return block;
	}
}

#endif
