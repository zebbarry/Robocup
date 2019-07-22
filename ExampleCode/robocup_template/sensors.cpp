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


// Convert analog value to mm
int convert_ir_dist(int analog, enum ir_type type) {
  int dist_mm;
  
  switch(type) {
    case SHORT:
    dist_mm = 18684*pow(analog, -0.952);  // Derived from excel using experimental data.
    if (dist_mm < 0 || dist_mm > 25000) {
      dist_mm = 25000;
    }
    break;
    
    case MEDIUM:
    dist_mm = 247397*pow(analog, -1.135);  // Derived from excel using experimental data.
    if (dist_mm < 0 || dist_mm > 25000) {
      dist_mm = 25000;
    }
    break;
  }
  
  return dist_mm;
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
  
  int actual = average_buf(&ir_array.left);
  ir_averages.left = convert_ir_dist(actual, SHORT);
  
  actual = average_buf(&ir_array.right);
  ir_averages.right = convert_ir_dist(actual, SHORT);
  
  actual = average_buf(&ir_array.front);
  ir_averages.front = convert_ir_dist(actual, MEDIUM);
  
  //Serial.println(ir_averages.left);
  //Serial.println(ir_averages.right);
  Serial.print("Sensor Value = ");
  Serial.print(actual);
  Serial.print("\t Dist Value = ");
  Serial.println(ir_averages.front);
  //Serial.println();
}
