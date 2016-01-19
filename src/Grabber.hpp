#ifndef Grabber_H
#define Grabber_H

#include "definitions.hpp"
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
			 * The Raspicam used for the class.
			 */
			PiCamera cam;
	};

	Grabber::Grabber(Zone zone, Side side) {

	}

	Grabber::~Grabber() {

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
}

#endif
