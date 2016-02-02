#include <opencv2/core/core.hpp>

#include "definitions.hpp"

namespace ChipChipArray {
	/**
	 *
	 */
	class Block {
		public:
			/**
			 *
			 */
			uint32 area;
			
			/**
			 *
			 */
			cv::Point bottomLeft;

			/**
			 *
			 */
			cv::Point bottomRight;

			/**
			 *
			 */
			uint16 height;

			/**
			 *
			 */
			cv::Point topLeft;

			/**
			 *
			 */
			cv::Point topRight;

			/**
			 *
			 */
			uint16 width;

			/**
			 *
			 */
			Color color;

			/**
			 *
			 */
			Size size;

			/**
			 *
			 */
			Block(cv::Rect rect, Color color);
	};

	Block::Block(cv::Rect rect, Color color) {
		area = rect.area();
		height = rect.height;
		width = rect.width;

		topLeft = rect.tl();
		bottomRight = rect.br();
		topRight = cv::Point(topLeft.x + width, topLeft.y);
		bottomLeft = cv::Point(topLeft.x, topLeft.y + height);
		this->color = color;
	}
}

