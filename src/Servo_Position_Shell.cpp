/* 
 * File:   Servo_Position_Shell.cpp
 * Author: Nickolas Neely
 *
 * Created on February 8, 2016, 12:05 PM
 */

#include <wiringPi.h>
#include "Adafruit_PWMServoDriver.h"
#include <iostream>
#include "Servo_Position_Shell.h"


// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servo_num;

/**
 * Desc: This function sets up the breakout board communication with I2C
 * using Adafruits_PWMServoDriver.cpp and to set the frequency of the 
 * servos to 60Hz.
 */
void setup() {
    //cout << "Testing Servos" << endl;
    pwm.begin();
    pwm.setPWMFreq(60.0); // Analog servos run at ~60 Hz updates
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!

void setServoPulse(uint8_t servo_num, double pulse) {
    double pulselength;

    pulselength = 1000000; // 1,000,000 us per second
    pulselength /= 60; // 60 Hz
    //cout << pulselength << " us per period" << endl;
    pulselength /= 4096; // 12 bits of resolution
    //cout << pulselength << "us per bit" << endl;
    pulse *= 1000;
    pulse /= pulselength;
    //cout << (uint16_t) pulse << endl;
    pwm.setPWM(servo_num, 0, (uint16_t) pulse);
    //cout << endl;
}

/**
 * Desc: This function sets which servo to use using whichservo and what 
 * position out of 180 degrees for each servo (with limits).
 * @param whichservo which servo would you like to use on the board
 * @param position what position do you want to set the servo selected at
 */
void setServoPosition(Servo whichservo, int position) {
    // works for servo 0, 3, 4
    double dividedconstant = 180.0;
    double highservo = 2.4;
    double lowservo = 0.6;
    // To fix the magical digital servo on LIFT 1
    double highservoweird = 1.9;
    double lowservoweird = 0.6;
    // To compensate for the bent servo spline on LIFT 2
    double highservospline = 2.25;
    double lowservospline = 0.6;
    // works for servo 1, 2
    double digitalservohigh = 2.45;
    double digitalservolow = 0.9;
    // left gripper servo 5
    double gripleftopen = 2.2;
    double gripleftclose = 1.5;
    // right gripper servo 6
    double griprightopen = 2.2;
    double griprightclose = 1.5;
    double pulse;

    switch (whichservo) {

            // BASE TURN
        case 0:
        {
            if (position == -1) {
                pulse = 0.0;
            }else if (position < 0){
                position = 20;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }else if (position > 179){
                position = 179;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }else{
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
            
        }

            break;

            // BASE TILT
        case 1:
        {
            
            
            if (position == -1) {
                pulse = 0.0;
            } else if (position < 90){
                position = 90;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else if (position > 172){
                position = 172;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;

            // ELBOW
        case 2:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if (position < 43){
                position = 43;
                pulse = ((((digitalservohigh - digitalservolow) / dividedconstant)*((double) position)) + digitalservolow);
            } else if (position > 179){
                position = 179;
                pulse = ((((digitalservohigh - digitalservolow) / dividedconstant)*((double) position)) + digitalservolow);
            } else {
                pulse = ((((digitalservohigh - digitalservolow) / dividedconstant)*((double) position)) + digitalservolow);
            }
        }
            break;

            // WRIST TURN
        case 3:
        {
            if (position == -1) {
                pulse = 0.0;
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;

            // WRIST PAN
        case 4:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if (position < 0){
                position = 0;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else if (position > 180){
                position = 180;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;

            // GRIP LEFT
        case 5:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((griprightopen - griprightclose) / dividedconstant)*((double) position)) + griprightclose);
            }else if(position > 90){
                position = 90;
                pulse = ((((griprightopen - griprightclose) / dividedconstant)*((double) position)) + griprightclose);
            } else {
                pulse = ((((gripleftopen - gripleftclose) / dividedconstant)*((double) position)) + gripleftclose);
            }
        }
            break;

            // GRIP RIGHT
        case 6:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 90){
                position = 90;
                pulse = ((((griprightopen - griprightclose) / dividedconstant)*((double) position)) + griprightclose);
            } else if(position > 180){
                position = 180;
                pulse = ((((griprightopen - griprightclose) / dividedconstant)*((double) position)) + griprightclose);
            }else{
                pulse = ((((griprightopen - griprightclose) / dividedconstant)*((double) position)) + griprightclose);
            }
        }
            break;

            // Michael Yellow Gate
        case 7:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else if(position > 90){
                position = 90;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;

            // Michael Green Gate
        case 8:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else if(position > 90){
                position = 90;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;

            // Michael Blue Gate
        case 9:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else if(position > 90){
                position = 90;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;

            // Michael Lift 1
        case 10:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((highservoweird - lowservoweird) / dividedconstant)*((double) position)) + lowservoweird);
            } else if(position > 105){
                position = 105;
                pulse = ((((highservoweird - lowservoweird) / dividedconstant)*((double) position)) + lowservoweird);
            } else {
                pulse = ((((highservoweird - lowservoweird) / dividedconstant)*((double) position)) + lowservoweird);
            }
        }
            break;

            //Michael Lift 2
        case 11:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((highservospline - lowservospline) / dividedconstant)*((double) position)) + lowservospline);
            } else if(position > 105){
                position = 105;
                pulse = ((((highservospline - lowservospline) / dividedconstant)*((double) position)) + lowservospline);
            } else {
                pulse = ((((highservospline - lowservospline) / dividedconstant)*((double) position)) + lowservospline);
            }
        }
            break;

            //Michael lift 3
        case 12:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else if(position > 105){
                position = 105;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;

            // Michael lift 4
        case 13:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else if(position > 105){
                position = 105;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;

            // Michael RED GATE
        case 14:
        {
            if (position == -1) {
                pulse = 0.0;
            } else if(position < 0){
                position = 0;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else if(position > 105){
                position = 105;
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            } else {
                pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            }
        }
            break;
    }
    setServoPulse(whichservo, pulse);

}
