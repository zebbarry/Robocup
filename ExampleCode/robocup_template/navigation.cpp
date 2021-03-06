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
bool in_front, to_left, to_right, left_closer, right_closer, to_right_extreme, to_left_extreme, ramp_present;
int32_t error_int, error_prev;
unsigned long last_change_t = 0;

// Navigation sequence
void navigate(void) {
  #if DEBUG
  Serial.println("Running navigation");
  #endif
  bool obstacle_present = false;
  ramp_present = !digitalRead(RAMP_L_PIN) || !digitalRead(RAMP_R_PIN);
  
  
  in_front     = ir_averages.front <= FRONT_LIMIT;
  to_left      = ir_averages.left  <= LEFT_LIMIT;
  to_right     = ir_averages.right <= RIGHT_LIMIT;
  left_closer  = ir_averages.left   < ir_averages.right;
  right_closer = ir_averages.left   > ir_averages.right;
  to_left_extreme      = ir_averages.left  <= LEFT_EXT_LIMIT;
  to_right_extreme     = ir_averages.right <= RIGHT_EXT_LIMIT;

  switch(robot_state) {
    case NO_WEIGHT:
      obstacle_present = obstacle_avoid();

      if (!obstacle_present) {
        if (cam_x < 1023) {
          weight_follow();
          led_toggle(BLUE);
        } else {
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



bool check_angle(float angle) {
  // rewturns true if angle is greater than max
  bool result = false;
  
  if (abs(angle) > MAX_FRWD_ANG) {
    if (angle > 0) {
      // Tilted backwards - approaching ramp 
      result = true;
    }
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
      angle = angle*2 + RGHT;
    } else {
      // Tilted left - ramp under right wheel
      angle = angle*2 + LEFT;
    }
  }
  return angle;
}


bool obstacle_avoid(void) {
  // Checks all direction avoiding obstacles if neccessary, returns false if no obstacles found.
  bool obstacle_present = true;
  unsigned long current_t, difference_t;
  static int blocked_ramp = 0;
  static int blocked_front = 0;
  static enum angle_s angle = FLAT;
  static enum angle_s prev_angle = FLAT;
  angle = check_ramp(imu_s2s, imu_f2b);
  #if DEBUG
  Serial.print("Checking angle: ");
  Serial.println(angle);
  #endif


  if ((angle == FLAT || angle == FRWD || angle == BACK) && blocked_ramp <= 0) {
    if (in_front || ramp_present) {    // Object in front          ------------------
      #if DEBUG
      Serial.println("Object in front");
      #endif
      
      if (to_left && to_right || ramp_present) { // Object in front and left and right
        #if DEBUG
        Serial.println("Cornered, reversing\n");
        #endif
        if (!(motor_speed_l == BACK_SLOW && motor_speed_r == BACK_SLOW)) {
          last_change_t = millis();
        }
        // Back up and turn around
        motor_speed_l = BACK_SLOW;
        motor_speed_r = BACK_SLOW;
        blocked_front = BLOCKED_DELAY;
        
      } else if (right_closer) {  // Object to right
        #if DEBUG
        Serial.println("Object also closer to the right, spinning left \n");
        #endif
        if (!(motor_speed_l == BACK_SLOW && motor_speed_r == FORWARD_SLOW)) {
          last_change_t = millis();
        }
        // Turn left
        motor_speed_l = BACK_SLOW;
        motor_speed_r = FORWARD_SLOW;
        
      } else {   // Object in front and closer to the left
        #if DEBUG
        Serial.println("Object also closer to the left, spinning right");
        #endif
        if (!(motor_speed_l == FORWARD_SLOW && motor_speed_r == BACK_SLOW)) {
          last_change_t = millis();
        }
        // Turn right
        motor_speed_l = FORWARD_SLOW;
        motor_speed_r = BACK_SLOW;
      }
    } else {                     // Nothing in front                        --------------------------
      if (to_left && to_right) { // In a channel
        if (blocked_front > 0) {
          #if DEBUG
          Serial.println("Obstacles left, right and previously in front. \n");
          #endif
          
          if (motor_speed_l == BACK_SLOW && motor_speed_r == BACK_SLOW) {   // Pick a direction and keep turning that way
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
          
        } else {
          #if DEBUG
          Serial.println("Obstacles left and right, driving forward slowly \n");
          #endif
          if (!(motor_speed_l == FORWARD_SLOW && motor_speed_r == FORWARD_SLOW)) {
            last_change_t = millis();
          }
          // Turn around
          motor_speed_l = FORWARD_SLOW;
          motor_speed_r = FORWARD_SLOW;
          blocked_front = 0;
        }
        
      } else if (to_left) { // Wall to left
        
        if (to_left_extreme) {
          #if DEBUG
          Serial.println("Object to the left, spinning right\n");
          #endif
          if (!(motor_speed_l == FORWARD_SLOW && motor_speed_r == BACK_SLOW)) {
            last_change_t = millis();
          }
          // Turn right a little bit
          motor_speed_l = FORWARD_SLOW;
          motor_speed_r = BACK_SLOW;
        } else {
          #if DEBUG
          Serial.println("Object to the left, turning right gradually \n");
          #endif
          if (!(motor_speed_l == FORWARD_SLOW && motor_speed_r == STOP_SPEED)) {
            last_change_t = millis();
          }
          // Turn right a little bit
          motor_speed_l = FORWARD_SLOW;
          motor_speed_r = STOP_SPEED;
        }
        
      } else if (to_right) { // Wall to right
        
        if (to_right_extreme) {
          #if DEBUG
          Serial.println("Object to the right, spinning left\n");
          #endif
          if (!(motor_speed_l == BACK_SLOW && motor_speed_r == FORWARD_SLOW)) {
            last_change_t = millis();
          }
          // Turn right a little bit
          motor_speed_l = BACK_SLOW;
          motor_speed_r = FORWARD_SLOW;
        } else {
          #if DEBUG
          Serial.println("Object to the right, turning left gradually \n");
          #endif
          if (!(motor_speed_l == STOP_SPEED && motor_speed_r == FORWARD_SLOW)) {
            last_change_t = millis();
          }
          // Turn right a little bit
          motor_speed_l = STOP_SPEED;
          motor_speed_r = FORWARD_SLOW;
        }
        
      } else {
        #if DEBUG
        Serial.println("No walls too close");
        #endif
        obstacle_present = false;
        blocked_front = 0;
      }
    }
  } else {                                // RAMP -------------
//    if (angle == BACK) {
//      #if DEBUG
//      Serial.println("Tilted back, reversing \n");
//      #endif
//      motor_speed_l = BACK_SLOW;
//      motor_speed_r = BACK_SLOW;
//      blocked_ramp = RAMP_DELAY + REVERSE_DELAY;
//      prev_angle = angle;
//      
//    } else 
    if (angle == BACK_RGHT) {
      #if DEBUG
      Serial.println("Tilted back and to the right, reversing while turning right \n");
      #endif
      motor_speed_l = BACK_SLOW - STEP;
      motor_speed_r = BACK_FULL;
      prev_angle = angle;
      
      
    } else if (angle == BACK_LEFT) {
      #if DEBUG
      Serial.println("Tilted back and to the left, reversing while turning left\n");
      #endif
      motor_speed_l = BACK_FULL;
      motor_speed_r = BACK_SLOW - STEP;
      prev_angle = angle;
      
    } else if (angle == LEFT || angle == FRWD_LEFT) {
      #if DEBUG
      Serial.println("Tilted to the left, turning left\n");
      #endif
      motor_speed_l = STOP_SPEED;
      motor_speed_r = FORWARD_SLOW;
      prev_angle = angle;
      
    } else if (angle == RGHT || angle == FRWD_RGHT) {
      #if DEBUG
      Serial.println("Tilted to the right, turning right\n");
      #endif
      motor_speed_l = FORWARD_SLOW;
      motor_speed_r = STOP_SPEED;
      prev_angle = angle;
      
  } else if (blocked_ramp == RAMP_DELAY) {    // Blocked ramp delay and finished reversing off ramp
      if (left_closer) {   // Object in front and closer to the left
        #if DEBUG
        Serial.println("Ramp infront and object closest to left, spinning right");
        #endif
        // Turn right
        motor_speed_l = FORWARD_SLOW;
        motor_speed_r = BACK_SLOW;
        
      } else {  // Object to right
        #if DEBUG
        Serial.println("Ramp infront and object closest to right, spinning left");
        #endif
        // Turn left
        motor_speed_l = BACK_SLOW;
        motor_speed_r = FORWARD_SLOW;
      }
    }
  }

  current_t = millis();
  difference_t = current_t - last_change_t;
  #if DEBUG
  Serial.println(difference_t);
  #endif

  if (difference_t > MAX_NO_CHANGE) {
    #if DEBUG
    Serial.println("WATCHDOG REACHED!! <-------------------------------\n");
    #endif
    led_on(RED);
    // Back up and turn around
    motor_speed_l = BACK_SLOW;
    motor_speed_r = BACK_SLOW;
    obstacle_present = true;
    
    if (difference_t > MAX_NO_CHANGE + 2000) {
      last_change_t = millis();
    }
  } else {
    led_off(RED);
  }


  if (blocked_front > 0) {
    blocked_front--;
  }
  if (blocked_ramp > 0) {
    blocked_ramp--;
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
      state_change = true;
    }
  }
}
