//************************************
//         sensors.cpp       
//************************************

 // This file contains functions used to read and average
 // the sensors.


#include <stdint.h>
#include "sensors.h"
#include "Arduino.h"
#include <CircularBuffer.h>

// Local definitions
ir_array_t ir_array;

void infrared_init(void) {
  Serial.println("Initialise infrared sensors \n");
  pinMode(IR_SHORT_LEFT_PIN, INPUT);
  pinMode(IR_SHORT_RIGHT_PIN, INPUT);
  pinMode(IR_SHORT_FRONT_PIN, INPUT);
}


int average_buf(CircularBuffer<int, IR_BUF_SIZE>* buf) {
  int sum = 0;

  for (int i = 0; i < buf->size()-1; i++) {
    sum += buf[0][i];
  }
  
  return (2*sum + buf->size()) / 2 / buf->size();
}


// Read ultrasonic value
void read_ultrasonic(/* Parameters */){
  Serial.println("Ultrasonic value \n");
}


// Read infrared value
void read_infrared(void){
  //Serial.println("Read infrared values \n");
  
  int sensorVal;
  sensorVal = analogRead(IR_SHORT_LEFT_PIN);
  ir_array.left.push(sensorVal);
  sensorVal = analogRead(IR_SHORT_RIGHT_PIN);
  ir_array.right.push(sensorVal);
  sensorVal = analogRead(IR_SHORT_FRONT_PIN);
  ir_array.front.push(sensorVal);
}


// Pass in data and average the lot
void sensor_average(void){
  //Serial.println("Averaging the sensors (L, R, F)");
  
  ir_averages.left = average_buf(&ir_array.left);
  ir_averages.right = average_buf(&ir_array.right);
  ir_averages.front = average_buf(&ir_array.front);
  
  Serial.println(ir_averages.left);
  Serial.println(ir_averages.right);
  Serial.println(ir_averages.front);
  Serial.println();
}
