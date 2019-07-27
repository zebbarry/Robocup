/************************************
 *        weight_collection.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  finding and collecting weights  */


#include "weight_collection.h"
#include "sensors.h"
#include "Arduino.h"

enum weight_s weight_state;

void weight_scan(void) 
{
  /* Use sensors to search for weights,
   * Switch to WEIGHT_FOUND state if a weight is found   */
   Serial.println("Looking for weights \n");

   int induct_state = digitalRead(INDUCTIVE_PIN);
   if (induct_state) {
    Serial.println("Inductive Active\n");
    weight_state = WEIGHT_FOUND;
   } else {
    weight_state = NO_WEIGHT;
   }
}


void collect_weight(void)
{
  /* When ready, collect the weight */

  if (weight_state == WEIGHT_FOUND) {
    Serial.println("Collecting weight \n");
    digitalWrite(MAG_PIN, HIGH);
  } else {
    digitalWrite(MAG_PIN, LOW);
  }
}
