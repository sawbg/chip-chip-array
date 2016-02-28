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
			 * The minimum block area in pixels to be classified
			 * as a whole (large) block
			 */
			static const uint32 MIN_WHOLE_BLOCK_SIZE = 50000;
	};

	Block::Block(cv::Rect rect, Color color) {
		area = rect.area();
		height = rect.height;
		width = rect.width;

		topLeft = rect.tl();
		bottomRight = rect.br();
		topRight = cv::Point(topLeft.x + width, topLeft.y);
		bottomLeft = cv::Point(topLeft.x, topLeft.y + height);
		offset = (sint16)(topLeft.x + width / 2) - 640;

		this->color = color;
		size = area > MIN_WHOLE_BLOCK_SIZE ? Size::Long : Size::Short;
	}
}
#endif
