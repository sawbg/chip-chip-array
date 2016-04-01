/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief Contains test code for arm.
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include "Servo_Position_Shell.cpp"

using namespace std;

/**
 * Controls the positions of the arm servos. Will likely work for other servos
 * on the robot, as well.
 *
 * Usage: arm [SERVO NUMBER] [ANGULAR POSITION DEGREES]
 */
int main(int argc, char** argv) {
	setup();
	
	if(argc != 3) {
		cout << "Usage: arm [SERVO] [VALUE]" << endl;
	} else {
		setServoPosition((Servo)atoi(argv[1]), atoi(argv[2]));
	}
}
