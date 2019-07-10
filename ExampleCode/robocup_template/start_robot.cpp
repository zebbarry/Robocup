//************************************
//         start_robot.cpp       
//************************************

 // This file contains functions used drive robot

#include <stdint.h>
#include <stdbool.h>
#include "start_robot.h"
#include "motors.h"
#include "sensors.h"
#include "Arduino.h"

// Local definitions

enum dir {LEFT = 0, RIGHT, FRONT};

// Start robot sequence
void start_robot(void){
  Serial.println("Checking directions \n");

  static bool sens_dir[NUM_IR_SENS];
  
  if (ir_averages.front > FRONT_LIMIT) {
    Serial.println("Object in front \n");
    sens_dir[FRONT] = true;
  } else if (sens_dir[FRONT]) {
    sens_dir[FRONT] = false;
    motor_speed_l = FORWARD_SPEED;
    motor_speed_r = FORWARD_SPEED;
  }
  
  if (ir_averages.left > LEFT_LIMIT) {
    Serial.println("Object to the left \n");
    sens_dir[LEFT] = true;
  } else if (sens_dir[LEFT]) {
    sens_dir[LEFT] = false;
    motor_speed_l = FORWARD_SPEED;
    motor_speed_r = FORWARD_SPEED;
  }
  
  if (ir_averages.right > RIGHT_LIMIT) {
    Serial.println("Object to the right \n");
    sens_dir[RIGHT] = true;
  } else if (sens_dir[RIGHT]) {
    sens_dir[RIGHT] = false;
    motor_speed_l = FORWARD_SPEED;
    motor_speed_r = FORWARD_SPEED;
  }
  
  if (sens_dir[LEFT]) {
    motor_speed_l += STEP;
    motor_speed_r -= STEP;
  }
  
  if (sens_dir[RIGHT]) {
    motor_speed_l -= STEP;
    motor_speed_r += STEP;
  }
  
  if (sens_dir[FRONT]) {
    motor_speed_l = STOP_SPEED;
    motor_speed_r = STOP_SPEED;
  }

  if (!sens_dir[LEFT] && !sens_dir[RIGHT] && !sens_dir[FRONT]) {
    motor_speed_l = FORWARD_SPEED;
    motor_speed_r = FORWARD_SPEED;
  }
}
