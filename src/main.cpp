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
#include "NavigationControl.h"
#include "Initialization.cpp"

//#define UNLOADTEST
//#define ARMTEST
//#define NAVTEST
//#define SWITCHTEST
#define READTHINKDO

using namespace std;

int startstatus;
int startroboth;
int stoproboth;
int track;
int navigationbusy;
int cleartonavigate;
int cleartoload;
int cleartounload;
int robotismoving;
int whereistherobot;
int loadcounter;
int unloadcounter;
int loadtheblocks;
int unloadtheblocks;
int giveitasec;
int startloadingthread;

int main() {

initialization();
#ifdef READTHINKDO
/*Track selection initialization will allow track select until start condition is true*/
while(1){
    while(track == 1 && startroboth == 0){
        startstatus = digitalRead(0);
        track = digitalRead(1);
        cout<<endl<<"I am in left track mode";
        if(startstatus == 1){
            startroboth = 1;
        }
        
        /* RUN MODE FOR LEFT TRACK GAME START!*/
        while(startroboth == 1 && stoproboth == 0){
 //READ for left track
            
            startstatus = digitalRead(0);
            navigationbusy = digitalRead(2);
            
//THINK for left track
            
            //clearance to do an action;
            if(cleartoload == 1 || cleartounload == 1){
                cleartonavigate = 0;
            }
            else if(cleartonavigate == 1 || cleartounload == 1){
                cleartoload = 0;
            }
            else if(cleartonavigate == 1 || cleartoload == 1){
                cleartounload = 0;
            }

            
            // latch to figure out robot position in sequence
            if(navigationbusy == 1 && robotismoving == 0){
                robotismoving = 1;
                whereistherobot++;
            }else if(navigationbusy == 0 && robotismoving == 1){
                robotismoving = 0;
            }
            
            //working actions 
            if(whereistherobot > 0 && whereistherobot <=4 && navigationbusy == 0){
                cleartoload = 1;
            }else if(whereistherobot > 0 && whereistherobot <=4 && navigationbusy == 1){
                cleartoload = 0;
            }else if(whereistherobot == 5 && navigationbusy == 0){
                cleartounload = 1;
            }else if(whereistherobot == 5 && navigationbusy == 1){
                cleartounload = 0;
            }
            

            //controls load action timing
            if(cleartoload == 1 && loadcounter < 2000){
                loadtheblocks = 1;
                loadcounter ++;
                startloadingthread = 1;
                
            }else if(loadcounter >= 2000){
                cleartoload = 0;
                loadcounter = 1;
            }
            
            //controls the unload action timing
            if(cleartounload ==1 && unloadcounter < 150){
                unloadcounter++;
                unloadtheblocks = 1;
            }else if(loadcounter >= 150){
                cleartounload = 0;
                unloadcounter = 1;
                unloadtheblocks = 0;
            }
            
//DO for the left track
            
            //Navigate to next position in sequence
            if(cleartounload == 0 && cleartoload == 0 && robotismoving == 0){
                digitalWrite(3,HIGH);  
            }else if(robotismoving == 1){
                digitalWrite(3,LOW);
            }
            
            //load blocks 
            if(loadtheblocks ==1){
                pthread_t andrewthread;
                pthread_create(&andrewthread,NULL,grabCall,NULL);
                
                loadtheblocks =0;
                
            }
            
            // unload the blocks
            if(unloadtheblocks ==1 && unloadcounter < 100){
                setServoPosition(Servo(10),90);
                setServoPosition(Servo(11),90);
                setServoPosition(Servo(12),90);
                setServoPosition(Servo(13),90);
            }else if(unloadtheblocks == 1 && unloadcounter >= 100 && unloadcounter <140){
                setServoPosition(Servo(10),0);
                setServoPosition(Servo(11),0);
                setServoPosition(Servo(12),0);
                setServoPosition(Servo(13),0);
            }else if(unloadtheblocks == 1 && unloadcounter >= 140){
                setServoPosition(Servo(10),-1);
                setServoPosition(Servo(11),-1);
                setServoPosition(Servo(12),-1);
                setServoPosition(Servo(13),-1);
            }
            
            cout<<endl<<"I am in run mode of left track";
            
            /* stops Robot if ever hit on a RTD loop*/
            if(startstatus == 0){
                digitalWrite(3,LOW);
                cout<<endl<<"Halting all function stop engaged after a start";
                giveitasec++;
                if(giveitasec >= 50){
                    stoproboth = 1;
                    giveitasec = 0;
                }
            }
            cout<<endl<<"The current position of robot is:"<<whereistherobot<<endl;
            
            // delay for 20 milliseconds per loop
            usleep(20000);


        }
    }
    /*Track selection initialization will allow track select until start condition is true*/
    while(track == 0 && startroboth ==0){
        startstatus = digitalRead(0);
        track = digitalRead(1);
        if(startstatus == 1){
            startroboth = 1;
        }
        cout<<endl<<"I am in right track mode.";
        cout<<"Not yet implemented. Danger. Danger. Danger.";
    }
}
#endif
#ifdef SWITCHTEST
    int cat;
    int dog;
    wiringPiSetup();
    while(1){
        
        //test each pin if needed for setup and checking
        /*
        cout<<endl;
        cout<<"Input a pin to check:"<<endl;
        cin>>dog;
        cat =digitalRead(dog);
        cout<<"I am reading:"<<cat<<endl;
        */
        
        if(digitalRead(0)==1){
            cout<<"Heavy is in stop or idle mode."<<endl;
           
        }else if(digitalRead(0)==0){
            cout<<"Heavy is in run mode!"<<endl;
        }
        if(digitalRead(1)==1){
            cout<<"Heavy is set for left track."<<endl;
        }else if(digitalRead(1)==0){
            cout<<"Heavy is set for right track."<<endl;
        }
        
        cout<<endl<<"Press enter to continue";
        cin.ignore();
    }
#endif
    
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

    int cat = 0;
    pinMode(2,INPUT);
    while(1){
        cout<<"High or Low?:"<<endl;
        cin>>cat;
        digitalWrite(3,cat);
        cout<<endl;
        if(digitalRead(2)==1){
            cout<<"I am getting a high from Micah"<<endl;
           
        }else if(digitalRead(2)==0){
            cout<<"I am getting a low from Micah"<<endl;
        }
    } 
    cin.ignore();

    
    
#endif

#ifdef UNLOADTEST
    
    Servo whichservo;
    int tmpServo = -1;
    int position;

    setup();
    while(1){
        /*
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
        */ 
        cout<<endl;
        cout<<"Pick a position (set position to -1 to disengage servo and set pwm to 0):";
        cin>>position;
        cout<<endl;
        //setServoPosition((Servo)tmpServo,position); 
        setServoPosition((Servo)10,position);
        setServoPosition((Servo)11,position);
        setServoPosition((Servo)12,position);
        setServoPosition((Servo)13,position);
        
    }
    
#endif
    return 0;
}
