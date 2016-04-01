/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief Contains Block class.
 */

#ifndef Block_H
#define Block_H

#include <opencv2/core/core.hpp>

#include "definitions.hpp"

namespace ChipChipArray {
	/**
	 * This class represents a block. It only works for blocks found with
	 * the "boundingRect" algorithm (i.e., it doesn't work for blocks that
	 * are skewed on the image).
	 */
	class Block {
		public:
			/**
			 * The area of the block in pixels
			 */
			uint32 area;

			/**
			 * Point of the block's bottom-left corner
			 */
			cv::Point bottomLeft;

			/**
			 * Point of the block's bottom-right corner
			 */
			cv::Point bottomRight;

			/**
			 * Number of pixels from the block's bottom edge to the bottom edge
			 * of the image frame.
			 */
			sint16 dBottom;

			/**
			 * Number of pixels from the block's left edge to the left edge of
			 * the image frame.
			 */
			sint16 dLeft;

			/**
			 * Number of pixels from the block's right edge to the right edge
			 * of the image frame.
			 */
			sint16 dRight;

			/**
			 * Number of pixels from the block's top edge to the top edge of the
			 * image frame.
			 */
			sint16 dTop;

			/**
			 * The difference between dTop and dBottom. It indicates the
			 * relative vertical positioning of the block regardless of the
			 * block's area. A positive value indicates the block is off-center
			 * towards the bottom.
			 */
			sint16 dTopBottom;

			/**
			 * The difference between dRight and dLeft. It indicates the
			 * relative vertical positioning of the block regardless of the
			 * block's area. A positive value indicates the block is off-center
			 * towards the left.
			 */
			sint16 dRightLeft;

			/**
			 * The difference in pixels between the vertical center of the image
			 * and the vertical center of the block. Assumes image is 1280
			 * pixels wide (like the Raspicam images).
			 */
			sint16 offset;

			/**
			 * The height of the block in pixels
			 */
			uint16 height;

			/**
			 * Point of the block's top-left corner
			 */
			cv::Point topLeft;

			/**
			 * Point of the block's top-right corner
			 */
			cv::Point topRight;

			/**
			 * The width of the block in pixels
			 */
			uint16 width;

			/**
			 * The detected color of the block
			 */
			Color color;

			/**
			 * The size of the block (half or whole)
			 */
			Size size;

			/**
			 * Creates a new Block using the Points in the cv::Rect
			 * and the color. Also determines the size based on the
			 * area of the Block.
			 */
			Block(cv::Rect rect, Color color);

		private:
			/**
			 * The image height in pixels.
			 */
			static const uint16 IMG_HEIGHT = 1280;

			/**
			 * The image width.
			 */
			static const uint16 IMG_WIDTH = 720;

			/**
			 * The minimum block area in pixels to be classified
			 * as a whole (large) block.
			 */
			static const uint32 MIN_WHOLE_BLOCK_SIZE = 50000;
	};

	Block::Block(cv::Rect rect, Color color) {
		// basic geometric properties
		area = rect.area();
		height = rect.height;
		width = rect.width;

		// assigning corners
		topLeft = rect.tl();
		bottomRight = rect.br();
		topRight = cv::Point(topLeft.x + width, topLeft.y);
		bottomLeft = cv::Point(topLeft.x, topLeft.y + height);
		offset = (sint16)(topLeft.x + width / 2) - IMG_WIDTH / 2;

		// calculating offsets (opencv low coordinates start top left)
		dLeft = topLeft.x;
		dRight = IMG_WIDTH - topRight.x;
		dTop = topLeft.y;
		dBottom = IMG_HEIGHT - bottomRight.y;
		dTopBottom = dTop - dBottom;
		dRightLeft = dRight - dLeft;

		// set color and size
		this->color = color;
		size = area > MIN_WHOLE_BLOCK_SIZE ? Size::Long : Size::Short;
	}
}
#endif
