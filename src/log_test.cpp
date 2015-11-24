#include <iostream>
#include <sstream>
#include <string>

#include "definitions.hpp"
#include "Log.hpp"

using namespace ChipChipArray;

int main() {
	Log log("/home/andrew/chip-chip-array/etc/logs/", LogMode::Text);
	log.Debug("yolo!");
	log.Variable("mymom","toldme");
}
