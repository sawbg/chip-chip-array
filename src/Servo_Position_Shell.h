/* 
 * File: Servo_Position_Shell.h
 * Author: Nickolas Neely
 *
 * Created on February 8, 2016, 12:05 PM
 */

#ifndef SERVO_POSITION_SHELL_H
#define	SERVO_POSITION_SHELL_H



#include <wiringPi.h>
#include "Adafruit_PWMServoDriver.h"
#include <iostream>
#include <ctype.h>
#include <cstdint>

/** Defines each of the servos on the robot.
 */
enum Servo{
    BASE_TURN = 0,
    BASE_TILT = 1,
    ELBOW = 2,
    WRIST_TILT = 3,
    WRIST_PAN = 4,
    GRIP_RIGHT = 5,
    GRIP_LEFT = 6,
    GATE_1 = 7,
    GATE_2 = 8,
    GATE_3 = 9,
    LIFT_1 = 10,
    LIFT_2 = 11,
    LIFT_3 = 12,
    LIFT_4 = 13,
    GATE_4 = 14
};

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Desc: This function sets which servo to use and what pulse to set that servos 
 * pwm to.
 * @param n which servo on the breakout board am I calling. Starting with 0.
 * @param pulse what is the pulse length (in micro seconds) the pwm of the servo
 * is set to. 
 */
void setServoPulse(uint8_t n, double pulse);
/**
 * Desc: This function sets up the breakout board communication with I2C
 * using Adafruits_PWMServoDriver.cpp and to set the frequency of the 
 * servos to 60Hz.
 */
void setup();
/**
 * Desc: This function sets which servo to use using whichservo and what 
 * position out of 180 degrees for each servo (with limits).
 * @param whichservo which servo would you like to use on the board
 * @param position what position do you want to set the servo selected at
 */
void setServoPosition(Servo whichservo, int position);

#ifdef __cplusplus
}
#endif 

#endif	/* SERVO_POSITION_SHELL_H */

