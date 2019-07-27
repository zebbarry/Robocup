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
int32_t error_int, error_prev;

// Start robot sequence
void start_robot(void){
  Serial.println("Checking directions");
  static bool i = false;
  if (i) {
    digitalWrite(33, HIGH);
  } else {
    digitalWrite(33, LOW);
  }
  i = !i;

  if (ir_averages.front < FRONT_LIMIT) {    // Object in front
    Serial.println("Object in front");
    if (ir_averages.left < LEFT_LIMIT && ir_averages.right < RIGHT_LIMIT) { // Cornered
      Serial.println("Cornered \n");
      // Back up and turn around
      motor_speed_l = BACK_SLOW;
      motor_speed_r = BACK_SLOW;
    } else if (ir_averages.left < ir_averages.right) {   // Object also to the left
      Serial.println("Object also to the left \n");
      // Turn right
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = BACK_SLOW;
      
    } else {  // Object to right
      Serial.println("Object also to the right \n");
      // Turn left
      motor_speed_l = BACK_SLOW;
      motor_speed_r = FORWARD_SLOW;
    }
    
  } else {
    if (ir_averages.left > LEFT_LIMIT && ir_averages.right > RIGHT_LIMIT) {   // No walls in sight
      Serial.println("No walls too close");
      if (ir_averages.left < ir_averages.right) { // Closer to the left
        Serial.println("Closer to the left \n");
        motor_speed_l = FORWARD_FULL - 10*STEP;
        motor_speed_r = FORWARD_FULL;
        
      } else if (ir_averages.left > ir_averages.right) {
        Serial.println("Closer to the right \n");
        motor_speed_l = FORWARD_FULL;
        motor_speed_r = FORWARD_FULL - 10*STEP;
      } else {
        Serial.println("No walls in sight \n");
        motor_speed_l = FORWARD_FULL;
        motor_speed_r = FORWARD_FULL;
      }
      
    } else if (ir_averages.left < LEFT_LIMIT) { // Wall to left
      Serial.println("Object to the left \n");
      // Turn right a little bit
      motor_speed_l = FORWARD_FULL;
      motor_speed_r = FORWARD_FULL - 10*STEP;
      
    } else if (ir_averages.right < RIGHT_LIMIT) {
      Serial.println("Object to the right \n");
      // Turn left a little bit
      motor_speed_l = FORWARD_FULL - 10*STEP;
      motor_speed_r = FORWARD_FULL;
    }
  }
}


void victory_dance(void) {
  Serial.println("Victory dance ");
  
}


int PID_control(int value, int desired, float Kp, float Ki, float Kd, int current_speed) {
  // Scales the values up by a constant so integers can be used. This removes rounding errors.
  int error = value - desired;

  // Proportional: The error times the proportional gain (Kp)
  int P = error * Kp;

  // Add the current Error to the error integral if current speed is less than maximum
  // and more than minimum. This removes integral windup.
  if (current_speed < FORWARD_FULL && current_speed > BACK_FULL)
  {
      error_int += (2 * error + DUTYSCALER) / 2 / DUTYSCALER;
  }

  // Integral: Multiply the error sum by the integral gain (Ki)
  int32_t I = Ki * error_int;

  // Derivative: Calculate change in error between now and previous operation,
  // then multiply by the differential gain (Kd)
  double D = Kd * (error_prev - error);
  error_prev = error;

  // Combine the proportional, integral and derivative components and then scales
  // back down using integer division. This reduces rounding error.
  int result = (2 * (P + I + D) + DUTYSCALER) / 2 / DUTYSCALER;

  return result;
}
