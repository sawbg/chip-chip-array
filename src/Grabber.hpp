/**
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief contains the Grabber class
 */

#ifndef Grabber_H
#define Grabber_H

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
#include "Servo_Position_Shell.cpp"

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
			 * Finds a single block of a particular color. This is intended to
			 * find a given when a block has already been chosen to be picked up
			 * and the camera has moved. To clarify, this function effectively
			 * finds only the new position of a given block.
			 *
			 * @param block the block to find
			 */
			Block FindBlock(Block block);

			/**
			 * Picks up block by examing how much it fills up the
			 * camera image and where its edges are. It is then
			 * decided where to grab the block, and the block is
			 * picked up.
			 *
			 * @param pos whether the (half-)block is closer to the
			 * robot (FRONT) or farther away (BACK).
			 */
			void Grab(BlockPosition pos);

			/**
			 * Picks up a block passed as an argument by examining the block's
			 * vertical offset from the vertical center of the image.
			 *
			 * @param block the block to be picked up
			 */
			void Grab(Block block);

			/**
			 * Finds the block to pick up. Color and block size are
			 * contained in this class.
			 *
			 * @eturn the block to pick up based on the Side given
			 */
			Block LocateBlocks();

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

		invokeCount++;

		this->zone = zone;
		this->side = side;

		log.Verbose("Setting HSV threshold values");

		rangeVals[Color::Red] = { cv::Scalar(0, 20, 20),
			cv::Scalar(12, 255, 255) };
		rangeVals[Color::Green] = { cv::Scalar(49, 0, 0),
			cv::Scalar(63, 255, 30) };
		rangeVals[Color::Blue] = { cv::Scalar(70, 0, 0),
			cv::Scalar(100, 255, 255) };

		/* Remember, we're only pretending this color's image is in HSV space.
		 * It's really in YUV, as required by Jacob yellow-detection algorithm. */
		rangeVals[Color::Yellow] = { cv::Scalar(0, 0, 0),
			cv::Scalar(255, 255, 5);
		}

		void Grabber::Close() {
			log.Status("Closing Grabber");
			cam.Close();
		}

		void Grabber::Discard() {

		}

		void Grabber::Extend() {

		}

		void Grabber::FindBlock(Block block) {

		}

		void Grabber::Grab(BlockPosition pos) {

		}

		void Grabber::Grab(Block block) {

		}

		Result Grabber::Load() {
			setServoPosition(BASE_TURN, 150);
			setServoPosition(WRIST_TILT, 50);
			setServoPosition(ELBOW, 140);
			sleep(2);
			Block b = LocateBlocks();
			int Q = 402;
			float32 a = 180 / 3.14152 * acos((float)b.offset / Q);
			if(a > 90) a -= 90;
			log.Debug("Angle a: " + to_string(a));
			setServoPosition(BASE_TURN, 150 + a);
			setServoPosition(WRIST_TILT, 50 - a);
			setServoPosition(ELBOW, 125);
			sleep(5);
			setServoPosition(BASE_TURN, 150);
			setServoPosition(WRIST_TILT, 50);
			setServoPosition(ELBOW, 140);
		}

		Block Grabber::LocateBlocks(Color color) {
			std::string logstr = "Locating blocks";

			if(color == Color::Perrywinkle) {
				logstr += " (" + std::to_string(color) + ")";
			}

			log.Verbose(logstr);

			cv::Mat imgOrig = cam.Snap();
			cv::transpose(cam.Snap(), imgOrig);

			cv::Mat imgHSV;
			cv::Mat imgThresh;
			std::vector<cv::Rect> blocks;
			std::vector<Color> colors;

			cv::cvtColor(imgOrig, imgHSV, cv::COLOR_BGR2HSV);

			uint8 loopNum = (color == Color::Perrywinkle ? rangeVals.size() : 1);

			for(int i = 0; i < loopNum;	i++) {
				if(color == Color::Perrywinkle) {
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
							cv::cvtColor(imgOrig, imgHSV, CV_BGR2YUV);
							break;
					}

					log.Verbose("Searching: " + std::to_string(color));

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
						cv::approxPolyDP(cv::Mat(contours[i]),
								contours_poly[i], 20,
								false);
						cv::Rect rect = cv::boundingRect(
								cv::Mat(contours_poly[i]));
						uint32 area = rect.width * rect.height;

						// determine if block and add to blocks vector
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
		}

#endif
