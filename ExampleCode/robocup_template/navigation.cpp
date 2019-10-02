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

// Local definitions
bool in_front, to_left, to_right, left_closer, right_closer;
int32_t error_int, error_prev;

// Navigation sequence
void navigate(void) {
  #if DEBUG
  Serial.println("Running navigation");
  #endif
//  Serial.println(cam_x);
  bool obstacle_present = false;

  in_front     = ir_averages.front <= FRONT_LIMIT;
  to_left      = ir_averages.left  <= LEFT_LIMIT;
  to_right     = ir_averages.right <= RIGHT_LIMIT;
  left_closer  = ir_averages.left   < ir_averages.right;
  right_closer = ir_averages.left   > ir_averages.right;

  switch(robot_state) {
    case NO_WEIGHT:
      obstacle_present = obstacle_avoid();

      if (!obstacle_present) {
        if (cam_x < 1023) {
          led_toggle(BLUE);
          weight_follow();
        } else {
          led_on(BLUE);
          wall_follow();
        }
      }
      break;

    case WEIGHT_AHEAD:
      obstacle_present = obstacle_avoid();

      if (!obstacle_present) {
        motor_speed_l = APPROACHING;
        motor_speed_r = APPROACHING;
      }
      break;

    case WEIGHT_FOUND:
      motor_speed_l = STOP_SPEED;
      motor_speed_r = STOP_SPEED;
      break;

    case COMP_OVER:
      motor_speed_l = STOP_SPEED;
      motor_speed_r = STOP_SPEED;
      break;
  }
}


enum angle_s check_ramp(float right2left, float back2front) {
  // Checks angle of robot to  identify ramps, tilt right and tilt backwards is +ve.
  int angle = FLAT;
  if (abs(back2front) > MAX_FRWD_ANG) {
    if (back2front > 0) {
      // Tilted backwards - approaching ramp 
      angle += BACK;
    } else {
      angle += FRWD;
    }
  }

  if (abs(right2left) > MAX_SIDE_ANG) {
    if (right2left > 0) {
      // Tilted right - ramp under left wheel
      if (angle != FLAT) {
        angle = angle*2 + RGHT;
      }
    } else {
      // Tilted left - ramp under right wheel
      if (angle != FLAT) {
        angle = angle*2 + LEFT;
      }
    }
  }
  return angle;
}


bool obstacle_avoid(void) {
  // Checks all direction avoiding obstacles if neccessary, returns false if no obstacles found.
  bool obstacle_present = true;
  static int blocked_front = 0;
  enum angle_s angle = check_ramp(imu_s2s, imu_f2b);
  #if DEBUG
  Serial.print("Checking angle: ");
  Serial.println(angle_s);
  #endif
  
  if (in_front) {    // Object in front
    #if DEBUG
    Serial.println("Object in front");
    #endif
    
    if (to_left && to_right) { // Object in front and left and right
      #if DEBUG
      Serial.println("Cornered, reversing\n");
      #endif
      // Back up and turn around
      motor_speed_l = BACK_SLOW;
      motor_speed_r = BACK_SLOW  ;
      blocked_front = BLOCKED_DELAY;
      
    } else if (left_closer) {   // Object in front and closer to the left
      #if DEBUG
      Serial.println("Object also closer to the left, spinning right");
      #endif
      // Turn right
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = BACK_SLOW;
      blocked_front = 0;
      
    } else {  // Object to right
      #if DEBUG
      Serial.println("Object also closer to the right, spinning left \n");
      #endif
      // Turn left
      motor_speed_l = BACK_SLOW;
      motor_speed_r = FORWARD_SLOW;
      blocked_front = 0;
    }
  } else {
    if (to_left && to_right) { // Cornered
      if (blocked_front) {
        #if DEBUG
        Serial.println("Obstacles left, right and previously in front. \n");
        #endif
        
        if (blocked_front == BLOCKED_DELAY) {   // Pick a direction and keep turning that way
          if (left_closer) {
            #if DEBUG
            Serial.println("Forced turning, spinning right");
            #endif
            // Turn right
            motor_speed_l = FORWARD_SLOW;
            motor_speed_r = BACK_SLOW;
            
          } else {
            #if DEBUG
            Serial.println("Right object closer, spinning left");
            #endif
            // Turn lwft
            motor_speed_l = BACK_SLOW;
            motor_speed_r = FORWARD_SLOW;
          }
        }
        
        blocked_front--;
        
      } else {
        #if DEBUG
        Serial.println("Obstacles left and right, driving forward slowly \n");
        #endif
        // Turn around
        motor_speed_l = FORWARD_SLOW;
        motor_speed_r = FORWARD_SLOW;
        blocked_front = 0;
      }
      
    } else if (to_left) { // Wall to left
      #if DEBUG
      Serial.println("Object to the left, turning right gradually \n");
      #endif
      // Turn right a little bit
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = BACK_SLOW;
      blocked_front = 0;
      
    } else if (to_right) {
      #if DEBUG
      Serial.println("Object to the right, turning left gradually \n");
      #endif
      // Turn left a little bit
      motor_speed_l = BACK_SLOW;
      motor_speed_r = FORWARD_SLOW;
      blocked_front = 0;
      
    } else {
      #if DEBUG
      Serial.println("No walls too close");
      #endif
      obstacle_present = false;
      blocked_front = 0;
    }
  }

  return obstacle_present;
}


void wall_follow(void) {
  #if DEBUG
  Serial.println("Following walls \n");
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


int calc_weight_error(void) {
  // Calc distance between current weight pos and desired.
  return DESIRED_POS - cam_x;
}


void weight_follow(void) {
  int error = calc_weight_error();
  bool at_max = false;
  int speed_change, speed_l, speed_r;
  
  speed_l = PID_HOLD;
  speed_r = PID_HOLD;

  if (motor_speed_l >= FORWARD_FULL || motor_speed_l <= BACK_FULL || motor_speed_r >= FORWARD_FULL || motor_speed_r <= BACK_FULL) {
    at_max = true;
  }

  if (abs(error) > ERROR_MARG) {
    speed_change = PID_control(error, KP, KI, KD, at_max);
    speed_l = speed_l - speed_change;
    speed_r = speed_r + speed_change;
  }

  motor_speed_l = speed_l;
  motor_speed_r = speed_r;

  #if DEBUG
  Serial.print("Calculating PID control (E, L, R): ");
  Serial.print(error);
  Serial.print(",");
  Serial.print(speed_l);
  Serial.print(",");
  Serial.println(speed_r);
  #endif
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
  if (robot_state == WEIGHT_AHEAD) {
    counter++;
    if (counter > MAX_WAIT) {
      counter = 0;
      robot_state = NO_WEIGHT;
    }
  }
}
