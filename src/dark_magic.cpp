/**
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief contains test code for arm
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include "Servo_Position_Shell.cpp"

using namespace std;

int main(int argc, char** argv) {
	setup();
	
	if(argc != 3) {
		cout << "Usage: arm [SERVO] [VALUE]" << endl;
	} else {
		setServoPosition((Servo)atoi(argv[1]), atoi(argv[2]));
	}
}
