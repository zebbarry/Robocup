//************************************
//         start_robot.cpp       
//************************************

 // This file contains functions used to return to and
 // detect bases

#include "start_robot.h"
#include "motors.h"
#include "sensors.h"
#include "Arduino.h"

// Local definitions
//#define 

// Start robot sequence
void start_robot(void){
  Serial.println("Checking for start signal \n");
  if (ir_averages.front < 20) {
    motor_speed = STOP_SPEED;
  } else {
    motor_speed = 1700;
  }
}
