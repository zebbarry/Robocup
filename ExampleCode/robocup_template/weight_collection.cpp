/************************************
 *        weight_collection.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  finding and collecting weights  */


#include "weight_collection.h"
#include "sensors.h"
#include "Arduino.h"
#include "stepper.h"
#include "debug.h"

enum weight_s weight_state;

void weight_scan(void) 
{
  /* Use sensors to search for weights,
   * Switch to WEIGHT_FOUND state if a weight is found   */
   #if DEBUG
   Serial.println("Looking for weights \n");
   #endif

   int induct_state = digitalRead(INDUCTIVE_PIN);
   if (induct_state) {
    #if DEBUG
    Serial.println("Inductive Active \n");
    #endif
    weight_state = WEIGHT_FOUND;
   } else if (WEIGHT_FOUND) {
    weight_state = NO_WEIGHT;
   }
}


void collect_weight(void)
{
  /* When ready, collect the weight */
  int induct_state;
   
  switch (weight_state) {
    case WEIGHT_FOUND:
    if (!collection_complete) {
      #if DEBUG
      Serial.println("Collecting weight \n");
      #endif
      digitalWrite(MAG_PIN, HIGH);
      delay(500);
      drive_step(STEPS, STEP_PIN);
      induct_state = digitalRead(INDUCTIVE_PIN);
      if (!induct_state) {
        #if DEBUG
        Serial.println("Inductive not active \n");
        #endif
        digitalWrite(DIR_PIN, HIGH);
        drive_step(STEPS, STEP_PIN);
        digitalWrite(DIR_PIN, LOW);
        weight_state = NO_WEIGHT;
      } else {
        collection_complete = true;
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
  digitalWrite(DIR_PIN, HIGH);
  drive_step(STEPS, STEP_PIN);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(MAG_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
  delay(100);
  collection_complete = false;
}
