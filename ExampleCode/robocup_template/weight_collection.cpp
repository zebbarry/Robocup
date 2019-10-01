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
  Q1 = true;
  Q2 = true;
  Q3 = true;
  start_t = millis();
  
  while(!hor_trig && current_t - start_t < MAX_TIME) {
    drive_step(CALIB_STEPS, HOR_STEP_PIN, HOR_DIR_PIN, LEFT);
    
    Q3 = Q2;
    Q2 = Q1;
    Q1 = !digitalRead(HOR_CALIB);
    if (Q1 && !Q2 && !Q3) {
      hor_trig = true;
    }    
    current_t = millis();
  }
  
  drive_step(HOR_RETURN, HOR_STEP_PIN, HOR_DIR_PIN, RIGHT);
  
  #if DEBUG
  if (hor_trig) {
    Serial.println("Horizontal stepper initialised");
  } else {
    Serial.println("Max time reached");
  }
  #endif
  
  Q1 = true;
  Q2 = true;
  Q3 = true;
  
  while(!ver_trig && current_t - start_t < MAX_TIME) {
    drive_step(CALIB_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN);

    Q3 = Q2;
    Q2 = Q1;
    Q1 = !digitalRead(VER_CALIB);
    if (Q1 && !Q2 && !Q3) {
      ver_trig = true;
    }
    current_t = millis();
  }
  
  drive_step(VER_RETURN, VER_STEP_PIN, VER_DIR_PIN, UP);
  
  #if DEBUG
  Serial.println("Vertical stepper initialised\n");
  #endif
}



void weight_scan(void) 
{
  /* Use sensors to search for weights,
   * Switch to WEIGHT_FOUND state if a weight is found   */
  #if DEBUG
  Serial.println("Looking for weights \n");
  #endif

  bool failed_state = false;
  int weight_in_channel, induct_state, delay_start, delay_current;
  
  weight_in_channel = !digitalRead(CHAN_PIN);
  if (weight_in_channel) {
    #if DEBUG
    Serial.println("Weight in channel! \n");
    #endif
    if (robot_state != WEIGHT_AHEAD) {
      robot_state = WEIGHT_AHEAD;
      state_change = true;
      left_motor.writeMicroseconds(STOP_SPEED);
      right_motor.writeMicroseconds(STOP_SPEED);
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
      robot_state = WEIGHT_FOUND;
      state_change = true;
    }
    
  } else if (robot_state == WEIGHT_FOUND){
    state_change = true;
    failed_state = true;
    delay_start = millis();
    robot_state = NO_WEIGHT;
    attempts = 0;
    
  } else if (failed_state) {
    delay_current = millis();
    if (delay_current - delay_start > FAILED_DELAY) {
      failed_state = false;
    }
  }
}


void collect_weight(void)
{
  /* When ready, collect the weight */
  int induct_state;
  
  switch (robot_state) {
    case WEIGHT_FOUND:
      if (!collection_complete) {
        #if DEBUG
        Serial.println("Collecting weight \n");
        #endif
        
        digitalWrite(MAG_PIN, HIGH);
        delay(500);
        drive_step(100, VER_STEP_PIN, VER_DIR_PIN, DOWN);
        drive_step(100, VER_STEP_PIN, VER_DIR_PIN, UP);
        drive_step(VER_STEPS, VER_STEP_PIN, VER_DIR_PIN, UP);
        induct_state = digitalRead(INDUCTIVE_PIN);
        
        if (!induct_state) {  // Pickup unsuccessful
          #if DEBUG
          Serial.println("Inductive not active \n");
          #endif
          drive_step(VER_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN);
          attempts++;
          
        } else {  // Pickup successful
          drive_step(HOR_STEPS, HOR_STEP_PIN, HOR_DIR_PIN, RIGHT);
          digitalWrite(MAG_PIN, LOW);
          drive_step(HOR_STEPS, HOR_STEP_PIN, HOR_DIR_PIN, LEFT);
          drive_step(VER_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN);
          
          #if DEBUG
          Serial.println("Collection complete \n");
          #endif
          state_change = true;
          robot_state = NO_WEIGHT;
          weight_count++;
          if (weight_count >= 2) {
            collection_complete = true;
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
  collection_complete = false;
}
