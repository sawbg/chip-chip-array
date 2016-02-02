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
	Grabber g(Zone::A, Side::Right);
	Block block = g.LocateBlock();

	std::cout << std::to_string(block.color) << std::endl;
}
