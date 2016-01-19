#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

#include "definitions.hpp"
#include "PiCamera.hpp"
#include "ScanQR.hpp"

using namespace ChipChipArray;

/**
 *
 */
int main() {
	while(true) {
		Color color = ScanQR();
		std::string colstr;

		switch(color) {
			case Color::Red:
				colstr = "RED";
				break;

			case Color::Yellow:
				colstr = "YELLOW";
				break;

			case Color::Green:
				colstr = "GREEN";
				break;

			case Color::Blue:
				colstr = "BLUE";
				break;

		}

		std::cout << colstr << std::endl;
	}
}
