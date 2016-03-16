/* 
 * File:   main.cpp
 * Author: Nick
 *
 * Created on February 1, 2016, 8:20 AM
 */

#include <cstdlib>
#include <unistd.h>
#include "Servo_Position_Shell.h"
#include "Adafruit_PWMServoDriver.h"
#include <iostream>
#include <map>
#include "NavigationControl.h"
//#define UNLOADTEST
#define ARMTEST
//#define NAVTEST
using namespace std;
/*
 * Test code for Servos 
 */

int main() {

#ifdef ARMTEST
    
    Servo whichservo;
    int tmpServo = -1;
    int position;
    setup();
    while(1){
        cout<<endl;
        cout<<"Pick a servo to use: BASE_TURN = 0, BASE_TILT = 1, ELBOW = 2, WRIST_TILT = 3,";
        cout<<endl<<"WRIST_PAN = 4, GRIP_LEFT = 5, GRIP_RIGHT = 6";
        cout<<endl;
        cin>>tmpServo;
        if(tmpServo > 6 || tmpServo < 0){
            cout<<"Please choose again:"<<endl;
            continue;
        }
        whichservo = (Servo)tmpServo;
        cout<<endl;
        cout<<"Pick a position (set position to -1 to disengage servo and set pwm to 0):";
        cin>>position;
        cout<<endl;
        setServoPosition(whichservo,position);
        
    }
#endif     

#ifdef NAVTEST
    
    int navaddress;
    int command;
    while(1){
        cout<<"What address should I write to?"<<endl;
        cin>>navaddress;
        cout<<"What number should I send boss? :"<<endl;
        cin>>command;
        cout<<"I sent this:"<<command<<endl;
        navigationSetup((uint8_t)navaddress);
        commandNavigation(navaddress);
    }
    
#endif

#ifdef UNLOADTEST
    
    Servo whichservo;
    int tmpServo = -1;
    int position;

    setup();
    while(1){
        
        cout<<endl;
        cout<<"Pick a servo to use: ";
        cout<<endl<<"UNLOAD_1= 10, UNLOAD_2 = 11, UNLOAD_3 = 12, UNLOAD_4 = 13";
        cout<<endl;
        cin>>tmpServo;
        if(tmpServo < 10 || tmpServo > 14){
            cout<<"Please choose again:"<<endl;
            continue;
        }
        whichservo = (Servo)tmpServo;
          
        cout<<endl;
        cout<<"Pick a position (set position to -1 to disengage servo and set pwm to 0):";
        cin>>position;
        cout<<endl;
        setServoPosition((Servo)tmpServo,position); 
        //setServoPosition((Servo)10,position);
        //setServoPosition((Servo)11,position);
        //setServoPosition((Servo)12,position);
        //setServoPosition((Servo)13,position);
        
    }
    
#endif
    
    return 0;
}
