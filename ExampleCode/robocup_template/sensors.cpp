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

void sensor_init(void) {
  Serial.println("Initialise sensors \n");
  pinMode(IR_SHORT_LEFT_PIN, INPUT);
  pinMode(IR_SHORT_RIGHT_PIN, INPUT);
  pinMode(IR_SHORT_FRONT_PIN, INPUT);
  pinMode(INDUCTIVE_PIN, INPUT_PULLUP);
  pinMode(US_TRIG_PIN, OUTPUT);
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
    if (analog < 35) {
      dist_mm = 633;
    }
    break;
    
    case MEDIUM:
    dist_mm = 213184*pow(analog, -1.125);  // Derived from excel using experimental data.
    if (analog < 50) {
      dist_mm = 2614;
    }
    break;
  }
  return dist_mm;
}


// Send ultrasonic value
void send_ultrasonic(void){
  Serial.println("Sending offensive ultrasonic pulse \n");
  static bool state = false;
  if (state) {
    digitalWrite(US_TRIG_PIN, HIGH);
  } else {
    digitalWrite(US_TRIG_PIN, LOW);
  }
}


// Read infrared value
void read_infrared(void){
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
  Serial.print("Averaging the sensors (L, R, F) ");
  
  int actual = average_buf(&ir_array.left);
  ir_averages.left = convert_ir_dist(actual, SHORT);
  
  actual = average_buf(&ir_array.right);
  ir_averages.right = convert_ir_dist(actual, SHORT);
  
  actual = average_buf(&ir_array.front);
  ir_averages.front = convert_ir_dist(actual, SHORT);
  
  Serial.print(ir_averages.left);
  Serial.print(" ");
  Serial.print(ir_averages.right);
  Serial.print(" ");
  Serial.println(ir_averages.front);
}
