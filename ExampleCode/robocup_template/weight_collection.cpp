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

enum weight_s weight_state;
int attempts;

void weight_scan(void) 
{
  /* Use sensors to search for weights,
   * Switch to WEIGHT_FOUND state if a weight is found   */
   #if DEBUG
   Serial.println("Looking for weights \n");
   #endif

   led_toggle(GREEN);

   int induct_state = digitalRead(INDUCTIVE_PIN);
   if (induct_state && attempts < MAX_ATTEMPTS) {
    #if DEBUG
    Serial.println("Inductive Active \n");
    #endif
    left_motor.writeMicroseconds(STOP_SPEED);
    right_motor.writeMicroseconds(STOP_SPEED);
    weight_state = WEIGHT_FOUND;
    collection_mode = true;
    state_change = true;
   } else if (weight_state == WEIGHT_FOUND){
    left_motor.writeMicroseconds(MAX_SPEED);
    right_motor.writeMicroseconds(MAX_SPEED);
    collection_mode = false;
    state_change = true;
    attempts = 0;
    weight_state = NO_WEIGHT;
   }
}


void collect_weight(void)
{
  /* When ready, collect the weight */
  int induct_state;

  led_toggle(RED);
  
  switch (weight_state) {
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
      
      if (!induct_state) {
        #if DEBUG
        Serial.println("Inductive not active \n");
        #endif
        drive_step(VER_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN);
        attempts += 1;
      } else {
        drive_step(HOR_STEPS, HOR_STEP_PIN, HOR_DIR_PIN, RIGHT);
        digitalWrite(MAG_PIN, LOW);
        drive_step(HOR_STEPS, HOR_STEP_PIN, HOR_DIR_PIN, LEFT);
        drive_step(VER_STEPS, VER_STEP_PIN, VER_DIR_PIN, DOWN);
        
        #if DEBUG
        Serial.println("Collection complete \n");
        #endif
//        collection_complete = true;
      }
    }
    break;

    case NO_WEIGHT:
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
