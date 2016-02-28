/* 
 * File:   Servo_Position_Shell.cpp
 * Author: Nickolas Neely
 *
 * Created on February 8, 2016, 12:05 PM
 */

#include <wiringPi.h>
#include "Adafruit_PWMServoDriver.h"
#include "Adafruit_PWMServoDriver.cpp"
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
    cout << pulselength << " us per period" << endl;
    pulselength /= 4096; // 12 bits of resolution
    cout << pulselength << "us per bit" << endl;
    pulse *= 1000;
    pulse /= pulselength;
    cout << (uint16_t) pulse << endl;
    pwm.setPWM(servo_num, 0, (uint16_t) pulse);
    cout << endl;
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
    // works for servo 1, 2
    double digitalservohigh = 2.45;
    double digitalservolow = 0.9;
    // left gripper servo 5
    double gripleftopen = 2.1;
    double gripleftclose = 1.5;
    // right gripper servo 6
    double griprightopen = 0.9;
    double griprightclose = 1.5;
    double pulse;

    switch (whichservo) {

            // BASE TURN
        case 0:
        {
            pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            if (pulse > highservo) {
                pulse = highservo;
            } else if (pulse < lowservo) {
                pulse = lowservo;
            }
            if (position == -1) {
                pulse = 0.0;
            }
        }

            break;

            // BASE TILT
        case 1:
        {
            pulse = ((((digitalservohigh - digitalservolow) / dividedconstant)*((double) position)) + digitalservolow);
            if (pulse > 1.75) {
                pulse = 1.75;
            } else if (pulse < 0.9) {
                pulse = 0.9;
            }
            if (position == -1) {
                pulse = 0.0;
            }
        }
            break;

            // ELBOW
        case 2:
        {
            pulse = ((((digitalservohigh - digitalservolow) / dividedconstant)*((double) position)) + digitalservolow);
            if (pulse > highservo) {
                pulse = highservo;
            } else if (pulse < 1.25) {
                pulse = 1.25;
            }
            if (position == -1) {
                pulse = 0.0;
            }
        }
            break;

            // WRIST TURN
        case 3:
        {
            pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            if (pulse > highservo) {
                pulse = highservo;
            } else if (pulse < lowservo) {
                pulse = lowservo;
            }
            if (position == -1) {
                pulse = 0.0;
            }
        }
            break;

            // WRIST PAN
        case 4:
        {
            pulse = ((((highservo - lowservo) / dividedconstant)*((double) position)) + lowservo);
            if (pulse > highservo) {
                pulse = highservo;
            } else if (pulse < lowservo) {
                pulse = lowservo;
            }
            if (position == -1) {
                pulse = 0.0;
            }
        }
            break;

            // GRIP LEFT
        case 5:
        {
            pulse = ((((gripleftopen - gripleftclose) / dividedconstant)*((double) position)) + gripleftclose);
            if (pulse > gripleftopen) {
                pulse = gripleftopen;
            } else if (pulse < gripleftclose) {
                pulse = gripleftclose;
            }
            if (position == -1) {
                pulse = 0.0;
            }
        }
            break;

            // GRIP RIGHT
        case 6:
        {
            pulse = ((((griprightopen - griprightclose) / dividedconstant)*((double) position)) + griprightclose);
            if (pulse > griprightclose) {
                pulse = griprightclose;
            } else if (pulse < griprightopen) {
                pulse = griprightclose;
            }
            if (position == -1) {
                pulse = 0.0;
            }
        }
            break;

            // Michael Yellow Gate
        case 7:
        {

        }
            break;

            // Michael Green Gate
        case 8:
        {

        }
            break;

            // Michael Blue Gate
        case 9:
        {

        }
            break;

            // Michael Red Gate
        case 10:
        {

        }
            break;

            //Michael lift Yellow
        case 11:
        {

        }
            break;

            //Michael lift Green
        case 12:
        {

        }
            break;

            // Michael lift Blue
        case 13:
        {

        }
            break;

            // Michael lift Red
        case 14:
        {

        }
            break;
    }
    setServoPulse(whichservo, pulse);

}
