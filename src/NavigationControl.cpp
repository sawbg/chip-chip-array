/**
 * @file
 * @author Nickolas Neely
 * @brief Contains the navigation control function definitions.
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
