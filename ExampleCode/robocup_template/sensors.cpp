//************************************
//         sensors.cpp       
//************************************

 // This file contains functions used to read and average
 // the sensors.


#include <stdint.h>
#include "sensors.h"
#include "Arduino.h"
#include <CircularBuffer.h>
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
#include "pin_map.h"
#include "imu.h"

// Local definitions
#define MIN_SHORT_ANALOG  30
#define MIN_MEDIUM_ANALOG 50

ir_array_t ir_array;
DFRobotVL53L0X tof_sensor;

void sensor_init(void) {
  #if DEBUG
  Serial.println("Initialise sensors \n");
  #endif
  pinMode(IR_SHORT_LEFT_PIN, INPUT);
  pinMode(IR_SHORT_RIGHT_PIN, INPUT);
  pinMode(IR_SHORT_FRONT_PIN, INPUT);
  pinMode(INDUCTIVE_PIN, INPUT_PULLUP);
  pinMode(US_TRIG_PIN, OUTPUT);
}


void tof_init(void) {
  #if DEBUG
  Serial.println("Initialise TOF camera");
  #endif

  tof_sensor.begin(TOF_ID);
  tof_sensor.setMode(Continuous, High);
  tof_sensor.start();
}


void read_tof(void) {
  #if DEBUG
  Serial.println("Reading TOF camera \n");
  #endif

  tof_reading = tof_sensor.getDistance();
}



void read_imu(void) {
  sEul_t imu_eul = read_imu_eul();
  
  imu_s2s = imu_eul.pitch;  // Tilt to right is +ve
  imu_f2b = imu_eul.roll;   // Tilt forwards is +ve
  
  #if DEBUG
  Serial.print("Reading IMU sensor (P, R): ");
  Serial.print(imu_s2s);
  Serial.print(", ");
  Serial.println(imu_f2b);
  #endif
}



int average_buf(CircularBuffer<int, IR_BUF_SIZE>* buf) {
  int sum = 0;
  for (int i = 0; i < buf->size()-1; i++) {
    sum += buf[0][i];
  }
  return (2*sum + buf->size()) / 2 / buf->size();
}



int convert_ir_dist_short(int analog)  {
  return 18684*pow(analog, -0.952);  // Derived from excel using experimental data.
}



int convert_ir_dist_medium(int analog)  {
  return 213184*pow(analog, -1.125);  // Derived from excel using experimental data.
}



// Convert analog value to mm
int convert_ir_dist(int analog, enum ir_type type) {
  int dist_mm;
  switch(type) {
    case SHORT:
    dist_mm = convert_ir_dist_short(analog);  // Derived from excel using experimental data.
    if (analog < MIN_SHORT_ANALOG) {
      dist_mm = convert_ir_dist_short(MIN_SHORT_ANALOG);
    }
    break;
    
    case MEDIUM:
    dist_mm = convert_ir_dist_medium(analog);  // Derived from excel using experimental data.
    if (analog < MIN_MEDIUM_ANALOG) {
      dist_mm = convert_ir_dist_medium(MIN_MEDIUM_ANALOG);
    }
    break;
  }
  return dist_mm;
}



// Send ultrasonic value
void send_ultrasonic(void){
  #if DEBUG
  Serial.println("Sending offensive ultrasonic pulse \n");
  #endif
  
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
  int average = average_buf(&ir_array.left);
  ir_averages.left = convert_ir_dist(average, SHORT);
  
  average = average_buf(&ir_array.right);
  ir_averages.right = convert_ir_dist(average, SHORT);
  
  average = average_buf(&ir_array.front);
  ir_averages.front = convert_ir_dist(average, SHORT);
  
  #if DEBUG
  Serial.print("Averaging the sensors (L, F, R) ");
  Serial.print(ir_averages.left);
  Serial.print(" ");
  Serial.print(ir_averages.front);
  Serial.print(" ");
  Serial.println(ir_averages.right);
  #endif
}
