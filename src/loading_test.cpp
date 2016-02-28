#include <iostream>

#include "definitions.hpp"
#include "Block.hpp"
#include "Grabber.hpp"

using namespace ChipChipArray;

/**
 * This program was used solely to test the Grabber class. It moves the arm and
 * picks up blocks.
 */
int main() {
	setup();
	Grabber g(Zone::C, Side::Left);
	g.Load();
	g.Close();
}
