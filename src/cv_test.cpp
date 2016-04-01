/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief Contains program used to test PiCamera class.
 */

#include <iostream>

#include "definitions.hpp"
#include "Block.hpp"
#include "Grabber.hpp"

using namespace ChipChipArray;

/**
 * This program was used solely to test the PiCamera wrapper class and its
 * compatibility with the raspicam wrapper and ultimately OpenCV.
 */
int main() {
	Grabber g(Zone::C, Side::Left);
	Block block = g.LocateBlock();
	g.Close();
	std::cout << std::to_string(block.color) << std::endl;
	std::cout << "Offset: " << block.offset << std::endl;

	Grabber g2(Zone::B, Side::Right);
	Block block2 = g2.LocateBlock();
	g2.Close();
	std::cout << std::to_string(block2.color) << std::endl;
	std::cout << "Offset: " << block2.offset << std::endl;

}
