/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief Contains the Arm class used to control the robotic arm.
 */

#ifndef Arm_H
#define Arm_H

#include <unistd.h>

#include "definitions.hpp"
#include "Servo_Position_Shell.cpp"

#define WRIST_TWIST WRIST_PAN

namespace ChipChipArray {
	/**
	 * This class provides a layer of abstraction from the existing servo
	 * interface. It is designed to make more sense programmatically and to be
	 * easier to use.
	 */
	class Arm {
		public:
			/**
			 * Initializes the I2C interface for the arm if another instance of
			 * the Arm class has not already.
			 */
			Arm();

			/**
			 * The instantaneous position of each arm servo.
			 */
			uint8 servoPos[7] = { 0, 0, 0, 0, 0, 0, 0 };

			/**
			 * Tilts the base of the arm.
			 *
			 * @param a desired servo position in degrees
			 */
			void BaseTilt(uint8 a);

			/**
			 * Twists the entire arm.
			 * 
			 * @param a desired servo position in degrees
			 */
			void BaseTurn(uint8 a);

			/**
			 * Opens the claw completely (within safe limits).
			 */
			void ClawOpen();

			/**
			 * Closes the claw enough to hold a block in place during movement
			 * but does not attempt to completely close the claw in order ot
			 * prevent unnecessary strain on the servos.
			 */
			void ClawClose();

			/**
			 * Tilts the base a certain number of degrees. 
			 *
			 * @param degrees to move servo. Positive values add to the servo
			 * angle, and negative values subtract from the servo angle.
			 */
			void dBaseTilt(sint16 a);

			/**
			 * Turn the base a certain number of degrees.
			 *
			 * @param degrees to move servo. Positive values add to the servo
			 * angle, and negative values subtract from the servo angle. 
			 */
			void dBaseTurn(sint16 a);

			/**
			 * Bend the elbow a certain number of degrees.
			 *
			 * @param degrees to move servo. Positive values add to the servo
			 * angle, and negative values subtract from the servo angle. 
			 */
			void dElbow(sint16 a);

			/**
			 * Tilt the wrist a certain number of degrees.
			 *
			 * @param degrees to move servo. Positive values add to the servo
			 * angle, and negative values subtract from the servo angle.
			 */
			void dWristTilt(sint16 a);

			/**
			 * Twist the wrist a certain number of degrees.
			 *
			 * @param degrees to move servo. Positive values add to the servo
			 * angle, and negative values subtract from the servo angle. 
			 */
			void dWristTwist(sint16 a);

			/**
			 * Bend the elbow to a specific position.
			 *
			 * @param a desired servo position in degrees
			 */
			void Elbow(uint8 a);

			/**
			 * Moves arm into its "hovering" position over the blocks. The
			 * position changes with the zone.
			 *
			 * @param zone the zone for which the arm should position itself
			 */
			void Hover(Zone zone);

			/**
			 * Tilt the wrist to a specific position.
			 *
			 * @param a desired servo position in degrees
			 */
			void WristTilt(uint8 a);

			/**
			 * Twist the wrist to a specific position.
			 *
			 * @param a desired servo position in degrees
			 */
			void WristTwist(uint8 a);

		protected:

			/**
			 * Moves the left gripper servo a certain number of degrees.
			 *
			 * @param degrees to move servo. Positive values add to the servo
			 * angle, and negative values subtract from the servo angle. 
			 */
			void dLeftGripper(sint16 a);

			/**
			 * Moves the right gripper servo a certain number of degrees.
			 *
			 * @param degrees to move servo. Positive values add to the servo
			 * angle, and negative values subtract from the servo angle. 
			 */
			void dRightGripper(sint16 a);

			/**
			 * Moves the left gripper to a specific position.
			 *
			 * @param a desired servo position in degrees
			 */
			void LeftGripper(uint8 a);

			/**
			 * Moves the right gripper to a specific position.
			 *
			 * @param a desired servo position in degrees
			 */
			void RightGripper(uint8 a);

		private:
			/**
			 * Whether the I2C connection has been initialized (i.e., setup()
			 * has been called).
			 */
			static bool init;
	};

	bool Arm::init = false;

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

	void Arm::ClawOpen() {
		LeftGripper(0);
		RightGripper(180);
	}

	void Arm::ClawClose() {
		LeftGripper(180);
		RightGripper(0);
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

	void Arm::dLeftGripper(sint16 a) {
		a += servoPos[GRIP_LEFT];
		setServoPosition(GRIP_LEFT, a);
		servoPos[GRIP_LEFT] = a;
	}

	void Arm::dRightGripper(sint16 a) {
		a += servoPos[GRIP_RIGHT];
		setServoPosition(GRIP_RIGHT, a);
		servoPos[GRIP_RIGHT] = a;
	}

	void Arm::dWristTilt(sint16 a) {
		a += servoPos[WRIST_TILT];
		setServoPosition(WRIST_TILT, a);
		servoPos[WRIST_TILT] = a;
	}

	void Arm::dWristTwist(sint16 a) {
		a += servoPos[WRIST_TWIST];
		setServoPosition(WRIST_TWIST, a);
		servoPos[WRIST_TWIST] = a;
	}

	void Arm::Elbow(uint8 a) {
		setServoPosition(ELBOW, a);
		servoPos[ELBOW] = a;
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

	void Arm::WristTwist(uint8 a) {
		setServoPosition(WRIST_TWIST, a);
		servoPos[WRIST_TWIST] = a;
	}
}

#endif
