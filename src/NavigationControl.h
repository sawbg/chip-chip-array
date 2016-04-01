/**
 * @file
 * @author Nickolas Neely
 * @date 22. Febrary 2016, 12:00 PM
 * @brief Contains the function definitions for navigation control.
 */

/* 
 * File:   NavigationControl.h
 * Author: Nick
 *
 * Created on February 22, 2016, 12:00 PM
 */


#ifndef NAVIGATIONCONTROL_H
#define NAVIGATIONCONTROL_H
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <ctype.h>

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifdef __cplusplus
extern "C"{
#endif


void navigationSetup(uint8_t navigation_add);

void commandNavigation(uint8_t navigationcommand);

#ifdef __cplusplus
}
#endif 

#endif /*NAVIGATIONCONTROL_H*/

