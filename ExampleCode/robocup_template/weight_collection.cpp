/************************************
 *        weight_collection.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  finding and collecting weights  */


#include "weight_collection.h"
#include "sensors.h"
#include "Arduino.h"
#include "stepper.h"
#include "motors.h"
#include "pin_map.h"
#include "led.h"

int attempts;


void gantry_init(void)
{
  #if DEBUG
  Serial.println("Initialising gantry");
  #endif

  bool hor_trig, ver_trig, Q1, Q2, Q3;
  int start_t, current_t;
  hor_trig = false;
  ver_trig = false;
  Q1 = false;
  Q2 = false;
  Q3 = false;
  start_t = millis();
  current_t = start_t + 1;  // To initialize
  
  while(!hor_trig && current_t - start_t < MAX_TIME) {
    drive_step(CALIB_STEPS, HOR_STEP_PIN, HOR_DIR_PIN, LEFT_S);
    
    Q3 = Q2;
    Q2 = Q1;
    Q1 = !digitalRead(HOR_CALIB);
    if (Q1 && !Q2 && !Q3) {
      hor_trig = true;
    }    
    current_t = millis();
  }
  
  drive_step(HOR_RETURN, HOR_STEP_PIN, HOR_DIR_PIN, RIGHT_S);
  
  #if DEBUG
  if (hor_trig) {
    Serial.println("Horizontal stepper initialised");
  } else {
    Serial.println("Max time reached");
  }
  #endif
  
  Q1 = false;
  Q2 = false;
  Q3 = false;
  
  while(!ver_trig && current_t - start_t < MAX_TIME) {
    drive_step(CALIB_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN_S);

    Q3 = Q2;
    Q2 = Q1;
    Q1 = !digitalRead(VER_CALIB);
    if (Q1 && !Q2 && !Q3) {
      ver_trig = true;
    }
    current_t = millis();
  }
  
  drive_step(VER_RETURN, VER_STEP_PIN, VER_DIR_PIN, UP_S);
  
  #if DEBUG
  if (ver_trig) {
    Serial.println("Vertical stepper initialised");
  } else {
    Serial.println("Max time reached");
  }
  #endif
}



void weight_scan(void) 
{
  /* Use sensors to search for weights,
   * Switch to WEIGHT_FOUND state if a weight is found   */
  #if DEBUG
  Serial.println("Looking for weights \n");
  #endif

  static bool failed_state = false;
  int weight_in_channel, induct_state;
  static unsigned long delay_start, delay_current;
  
  weight_in_channel = !digitalRead(CHAN_PIN);
  if (weight_in_channel) {
    #if DEBUG
    Serial.println("Weight in channel! \n");
    #endif
    if (robot_state != WEIGHT_AHEAD) {
      robot_state = WEIGHT_AHEAD;
      state_change = true;
      gantry_init();
    }
  }
  
  induct_state = digitalRead(INDUCTIVE_PIN);
  if (induct_state && attempts < MAX_ATTEMPTS && !failed_state) {
    #if DEBUG
    Serial.println("Inductive Active \n");
    #endif

    if (robot_state != WEIGHT_FOUND) {
      left_motor.writeMicroseconds(STOP_SPEED);
      right_motor.writeMicroseconds(STOP_SPEED);
      motor_speed_l = STOP_SPEED;
      motor_speed_r = STOP_SPEED;
      robot_state = WEIGHT_FOUND;
      state_change = true;
    }
    
  } else if (attempts >= MAX_ATTEMPTS && !failed_state){
    #if DEBUG
    Serial.println("Max attempts reached");
    #endif
    state_change = true;
    failed_state = true;
    delay_start = millis();
    robot_state = NO_WEIGHT;
    attempts = 0;
    
  } else if (failed_state) {
    delay_current = millis();
    if (delay_current - delay_start > FAILED_DELAY) {
      #if DEBUG
      Serial.println("Delay limit reached");
      Serial.println(delay_current);
      Serial.println(delay_start);
      #endif
      failed_state = false;
    } 
  } else if (robot_state == WEIGHT_FOUND) {
      #if DEBUG
      Serial.println("Inductive inactive, changing state");
      #endif
      state_change = true;
      robot_state = NO_WEIGHT;
      attempts = 0;
  }
}


void collect_weight(void)
{
  /* When ready, collect the weight */
  int induct_state;
  
  switch (robot_state) {
    case WEIGHT_FOUND:
      #if DEBUG
      Serial.println("Collecting weight \n");
      #endif
      if (digitalRead(INDUCTIVE_PIN)) {
        
        digitalWrite(MAG_PIN, HIGH);
        delay(500);
        drive_step(PUSH_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN_S);
        drive_step(PUSH_STEPS, VER_STEP_PIN, VER_DIR_PIN, UP_S);
        drive_step(VER_STEPS, VER_STEP_PIN, VER_DIR_PIN, UP_S);
        induct_state = digitalRead(INDUCTIVE_PIN);
        
        if (!induct_state) {  // Pickup unsuccessful
          drive_step(VER_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN_S);
          attempts++;
          #if DEBUG
          Serial.println("Inductive not active");
          Serial.print("Attempt: ");
          Serial.println(attempts);
          #endif
          
        } else {  // Pickup successful
          drive_step(HOR_STEPS, HOR_STEP_PIN, HOR_DIR_PIN, RIGHT_S);
          drive_step(DROP_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN_S);
          digitalWrite(MAG_PIN, LOW);
          drive_step(DROP_STEPS, VER_STEP_PIN, VER_DIR_PIN, UP_S);
          drive_step(HOR_STEPS, HOR_STEP_PIN, HOR_DIR_PIN, LEFT_S);
          drive_step(VER_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN_S);
          
          #if DEBUG
          Serial.println("Collection complete \n");
          #endif
          state_change = true;
          robot_state = NO_WEIGHT;
          weight_count++;
          if (weight_count >= MAX_WEIGHTS) {
            robot_state = COMP_OVER;
            state_change = true;
          }
          attempts = 0;
        }
      }
      break;

    default:
      digitalWrite(MAG_PIN, LOW);
      break;
  }
}


void victory_dance(void) {
  #if DEBUG
  Serial.println("Victory dance ");
  #endif
  digitalWrite(FAN_PIN, HIGH);
  delay(5000);
  digitalWrite(FAN_PIN, LOW);
  delay(1000);
}
