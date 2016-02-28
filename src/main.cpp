/* 
 * File:   main.cpp
 * Author: Nick
 *
 * Created on February 1, 2016, 8:20 AM
 */

#include <cstdlib>
#include <unistd.h>
#include "Servo_Position_Shell.h"
#include <iostream>
#include <map>
using namespace std;
/*
 * Test code for Servos 
 */

int main() {
 
    Servo whichservo;
    int tmpServo = -1;
    int position;
    setup();
    while(1){
        cout<<endl;
        cout<<"Pick a servo to use: BASE_TURN = 0, BASE_TILT = 1, ELBOW = 2, WRIST_TILT = 3,";
        cout<<endl<<"WRIST_PAN = 4, GRIP_LEFT = 5, GRIP_RIGHT = 6, GATE_YELLOW = 7, GATE_GREEN = 8";
        cout<<endl<<"GATE_BLUE = 9, GATE_RED = 10, LIFT_YELLOW = 11, LIFT_GREEN = 12, LIFT_BLUE = 13";
        cout<<endl<<" LIFT_RED = 14;";
        cout<<endl;
        cin>>tmpServo;
        whichservo = (Servo)tmpServo;
        cout<<endl;
        cout<<"Pick a position (set position to -1 to disengage servo and set pwm to 0):";
        cin>>position;
        cout<<endl;
        setServoPosition(whichservo,position);
        
    }
        
    
    return 0;
}