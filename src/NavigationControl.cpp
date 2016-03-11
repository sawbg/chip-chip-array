/*
 * File: NavigationControl.cpp
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "NavigationControl.h"
    
//uint8_t navigation_address = 10u;
uint8_t navigationcmd;
uint8_t nav_fd; 


void navigationSetup(uint8_t navigation_add){
    nav_fd=wiringPiI2CSetup(navigation_add);
    
}

void commandNavigation(uint8_t navigationcommand){
    uint8_t data = navigationcommand;
    wiringPiI2CWrite(nav_fd, data);
}
