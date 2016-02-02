#include <iostream>
#include <sstream>
#include <string>

#include "definitions.hpp"
#include "Log.hpp"

using namespace ChipChipArray;
/**
 * This program partially tests the Log class.
 */
int main() {
	Log log("logs", LogMode::Text);
	log.Debug("yolo!");
	log.Variable("mymom","toldme");
}
