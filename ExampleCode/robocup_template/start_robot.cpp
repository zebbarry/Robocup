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
#include "pin_map.h"
#include "led.h"

// Local definitions
int32_t error_int, error_prev;

// Start robot sequence
void start_robot(void){
  #if DEBUG
  Serial.println("Checking directions");
  #endif

  led_toggle(BLUE);

  if (ir_averages.frlt < FRONT_LIMIT || ir_averages.frrt < FRONT_LIMIT) {    // Object in front
    #if DEBUG
    Serial.println("Object in front");
    #endif
    if (ir_averages.left < LEFT_LIMIT && ir_averages.right < RIGHT_LIMIT) { // Object in front and left and right
      #if DEBUG
      Serial.println("Cornered \n");
      #endif
      // Back up and turn around
      motor_speed_l = BACK_SLOW;
      motor_speed_r = BACK_SLOW;
      
    } else if (ir_averages.left < ir_averages.right) {   // Object in front and closer to the left
      #if DEBUG
      Serial.println("Object also closer to the left");
      #endif
      // Turn right
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = BACK_SLOW;
      
    } else {  // Object to right
      #if DEBUG
      Serial.println("Object also closer to the right \n");
      #endif
      // Turn left
      motor_speed_l = BACK_SLOW;
      motor_speed_r = FORWARD_SLOW;
    }
    
  } else {
    if (ir_averages.left > LEFT_LIMIT && ir_averages.right > RIGHT_LIMIT) {   // No walls in sight
      #if DEBUG
      Serial.println("No walls too close");
      #endif
      
      if (ir_averages.left < ir_averages.right) { // Closer to the left
        #if DEBUG
        Serial.println("Closer to the left \n");
        #endif
        motor_speed_l = FORWARD_FULL - STEP;
        motor_speed_r = FORWARD_FULL;
        
      } else if (ir_averages.left > ir_averages.right) {
        #if DEBUG
        Serial.println("Closer to the right \n");
        #endif
        motor_speed_l = FORWARD_FULL;
        motor_speed_r = FORWARD_FULL - STEP;
      } else {
        #if DEBUG
        Serial.println("No walls in sight \n");
        #endif
        motor_speed_l = FORWARD_FULL;
        motor_speed_r = FORWARD_FULL;
      }
    } else if (ir_averages.left < LEFT_LIMIT && ir_averages.right < RIGHT_LIMIT) { // Cornered
      #if DEBUG
      Serial.println("Cornered with nothing in front \n");
      #endif
      // Turn around
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = FORWARD_SLOW;
       
    } else if (ir_averages.left < LEFT_LIMIT) { // Wall to left
      #if DEBUG
      Serial.println("Object to the left \n");
      #endif
      // Turn right a little bit
      motor_speed_l = FORWARD_FULL;
      motor_speed_r = STOP_SPEED;
      
    } else if (ir_averages.right < RIGHT_LIMIT) {
      #if DEBUG
      Serial.println("Object to the right \n");
      #endif
      // Turn left a little bit
      motor_speed_l = STOP_SPEED;
      motor_speed_r = FORWARD_FULL;
    } else {
      #if DEBUG
      Serial.println("WTFFFFFFFFFF \n");
      #endif
      // Turn left a little bit
      led_toggle(RED);
      motor_speed_l = STOP_SPEED;
      motor_speed_r = STOP_SPEED;
      
    }
  }
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
