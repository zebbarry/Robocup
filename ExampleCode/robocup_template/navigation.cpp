//************************************
//         navigation.cpp       
//************************************

 // This file contains functions used drive robot

#include <stdint.h>
#include <stdbool.h>
#include "navigation.h"
#include "motors.h"
#include "sensors.h"
#include "Arduino.h"
#include "irposition.h"
#include "pin_map.h"
#include "led.h"
#include "weight_collection.h"

// Local definitions
bool in_front, to_left, to_right, left_closer, right_closer;
int32_t error_int, error_prev;

// Navigation sequence
void navigate(void) {
  #if DEBUG
  Serial.println("Checking directions");
  #endif

  switch(robot_state) {
    case NO_WEIGHT:
      //wall_follow();
    
      if (cam_x < 1023) {
        weight_follow();
      }
      break;

    case WEIGHT_AHEAD:
      wall_follow();
      
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = FORWARD_SLOW;
  }
}


void wall_follow(void) {
  in_front     = ir_averages.front <= FRONT_LIMIT;
  to_left      = ir_averages.left  <= LEFT_LIMIT;
  to_right     = ir_averages.right <= RIGHT_LIMIT;
  left_closer  = ir_averages.left   < ir_averages.right;
  right_closer = ir_averages.left   > ir_averages.right;
  
  
  if (in_front) {    // Object in front
    #if DEBUG
    Serial.println("Object in front");
    #endif
    
    if (to_left && to_right) { // Object in front and left and right
      #if DEBUG
      Serial.println("Cornered \n");
      #endif
      // Back up and turn around
      motor_speed_l = BACK_SLOW;
      motor_speed_r = BACK_SLOW;
      
    } else if (left_closer) {   // Object in front and closer to the left
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
    if (to_left && to_right) { // Cornered
      #if DEBUG
      Serial.println("Cornered with nothing in front \n");
      #endif
      // Turn around
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = FORWARD_SLOW;
       
    } else if (to_left) { // Wall to left
      #if DEBUG
      Serial.println("Object to the left \n");
      #endif
      // Turn right a little bit
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = STOP_SPEED;
      
    } else if (to_right) {
      #if DEBUG
      Serial.println("Object to the right \n");
      #endif
      // Turn left a little bit
      motor_speed_l = STOP_SPEED;
      motor_speed_r = FORWARD_SLOW;
    } else {
      #if DEBUG
      Serial.println("No walls too close");
      #endif
      
      if (left_closer) { // Closer to the left
        #if DEBUG
        Serial.println("Closer to the left \n");
        #endif
        motor_speed_l = FORWARD_FULL - STEP;
        motor_speed_r = FORWARD_FULL;
        
      } else if (right_closer) {
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
    }
  }
}


int calc_weight_error(void) {
  // Calc distance between current weight pos and desired.
  return DESIRED_POS - cam_x;
}


void weight_follow(void) {
  int error = calc_weight_error();
  bool at_max = false;
  int speed_change, speed_l, speed_r;

  if (motor_speed_l >= FORWARD_FULL || motor_speed_l <= BACK_FULL || motor_speed_r >= FORWARD_FULL || motor_speed_r <= BACK_FULL) {
    at_max = true;
  }

  if (abs(error) > ERROR_MARG) {
    speed_change = PID_control(error, KP, KI, KD, at_max);
    speed_l = PID_HOLD + speed_change;
    speed_r = PID_HOLD - speed_change;
  }

  Serial.print("Calculating PID control (E, L, R): ");
  Serial.print(error);
  Serial.print(",");
  Serial.print(speed_l);
  Serial.print(",");
  Serial.println(speed_r);
}


int PID_control(int error, float Kp, float Ki, float Kd, bool at_max) {
  // Proportional: The error times the proportional gain (Kp)
  int P = error * Kp;

  // Add the current Error to the error integral if current speed is less than maximum
  // and more than minimum. This removes integral windup.
  if (!at_max)
  {
      error_int += error;
  }

  // Integral: Multiply the error sum by the integral gain (Ki)
  int32_t I = Ki * error_int;

  // Derivative: Calculate change in error between now and previous operation,
  // then multiply by the differential gain (Kd)
  double D = Kd * (error_prev - error);
  error_prev = error;

  // Combine the proportional, integral and derivative components and then scales
  // back down using integer division. This reduces rounding error.
  int result = P + I + D;

  return result;
}

void check_watchdog(void) {
  static int counter;
  counter++;
  if (counter > MAX_WAIT) {
    counter = 0;
    robot_state = NO_WEIGHT;
  }
}
