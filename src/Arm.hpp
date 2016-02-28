/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief contains the Arm class used to control the robotic arm
 */

#ifndef Arm_H
#define Arm_H

#include "definitions.hpp"
#include "Servo_Position_Shell.cpp"

namespace ChipChipArray {
	/**
	 *
	 */
	class Arm {
		public:
			/**
			 * Initializes the I2C interface for the arm if another instance of
			 * the Arm class has not already.
			 */
			Arm();

			/**
			 * Tilts the base of the arm.
			 *
			 * @param a desired servo position in degrees
			 */
			void BaseTilt(uint8 a);

			/**
			 * Twists the entire arm.
			 *
			 * 
			 */
			void BaseTurn(uint8 a);

			/**
			 * 
			 */
			void dBaseTilt(uint8 a);

			/**
			 *
			 */
			void dBaseTurn(uint8 a);

			/**
			 *
			 */
			void dElbow(uint8 a);

			/**
			 *
			 */
			void dGrippers(uint8 a);

			/**
			 *
			 */
			void dWristTilt(uint8 a);

			/**
			 *
			 */
			void dWristTwist(uint8 a);

			/**
			 *
			 */
			void Elbow(uint8 a);

			/**
			 *
			 */
			void Grippers(uint8 a);

			/**
			 *
			 */
			void Hover(Zone zone);

			/**
			 *
			 */
			void WristTilt(uint8 a);

			/**
			 *
			 */
			void WristTwist(uint8 a);

		protected:
			/**
			 *
			 */
			uint8 servoPos[] = { 0, 0, 0, 0, 0, 0, 0 };

			/**
			 *
			 */
			void dLeftGripper(sint16 a);

			/**
			 *
			 */
			void dRightGripper(sint16 a);

			/**
			 *
			 */
			void LeftGripper(uint8 a);

			/**
			 *
			 */
			void RightGripper(uint8 a);

		private:
			/**
			 *
			 */
			static bool init = false;
	};

	Arm::Arm() {
		if(!init) {
			setup();
			init = true;
		}
	}

	void Arm::BaseTilt(uint8 a) {
		setServoPosition(BASE_TILT, a);
		servoPos[BASE_TILT] = a;
	}

	void Arm::BaseTurn(uint8 a) {
		setServoPosition(BASE_TURN, a);
		servoPos[BASE_TURN] = a;
	}

	void Arm::dBaseTilt(sint16 a) {
		a += servoPos[BASE_TILT];
		setServoPosition(BASE_TILT, a);
		servoPos[BASE_TILT] = a;
	}

	void Arm::dBaseTurn(sint16 a) {
		a += servoPos[BASE_TURN];
		setServoPosition(BASE_TURN, a);
		servoPos[BASE_TURN] = a;
	}

	void Arm::dElbow(sint16 a) {
		a += servoPos[ELBOW];
		setServoPosition(ELBOW, a);
		servoPos[ELBOW] = a;
	}

	void Arm::dGrippers(sint16 a) {
		/*	a += servoPos[BASE_TILT];
			setServoPosition(BASE_TILT, a);
			servoPos[BASE_TILT] = a;
			*/
	}

	void Arm::dLeftGripper(uint8 a) {
		a += servoPos[GRIP_LEFT];
		setServoPosition(GRIP_LEFT, a);
		servoPos[GRIP_LEFT] = a;
	}

	void Arm::dRightGripper(uint8 a) {
		a += servoPos[GRIP_RIGHT];
		setServoPosition(GRIP_RIGHT, a);
		servoPos[GRIP_RIGHT] = a;
	}

	void Arm::dWristTilt(sint16 a) {
		a += servoPos[WRIST_TILT];
		setServoPosition(WRIST_TILT, a);
		servoPos[WRIST_TILT] = a;
	}

	void Arm::dWristTurn(sint16 a) {
		a += servoPos[WRIST_TURN];
		setServoPosition(WRIST_TURN, a);
		servoPos[WRIST_TURN] = a;
	}

	void Arm::Elbow(uint8 a) {
		setServoPosition(ELBOW, a);
		servoPos[ELBOW] = a;
	}

	void Arm::Grippers(uint8 a) {
		// implement later	
	}

	void Arm::Hover(Zone zone) {
		// implement later
	}

	void Arm::LeftGripper(uint8 a) {
		setServoPosition(GRIP_LEFT, a);
		servoPos[GRIP_LEFT] = a;
	}

	void Arm::RightGripper(uint8 a) {
		setServoPosition(GRIP_RIGHT, a);
		servoPos[GRIP_RIGHT] = a;
	}

	void Arm::WristTilt(uint8 a) {
		setServoPosition(WRIST_TILT, a);
		servoPos[WRIST_TILT] = a;
	}

	void Arm::WristTurn(uint8 a) {
		setServoPosition(WRIST_TURN, a);
		servoPos[WRIST_TURN] = a;
	}
}

#endif
