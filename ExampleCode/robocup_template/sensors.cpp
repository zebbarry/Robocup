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

enum ir_dir {LEFT = 0, RIGHT, FRONT, NUM_IR_SENS};

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
  imu_reading = read_imu_eul_dir(ROLL);
  
  #if DEBUG
  Serial.print("Reading IMU sensor: ");
  Serial.println(imu_reading);
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
  
   static int limit_1, limit_2, limit_3;
  
   limit_3 = limit_2;
   limit_2 = limit_1;
   limit_1 = !digitalRead(LIMIT_PIN);
   
   if (!limit_3 && !limit_2 && limit_1) {
    #if DEBUG
    Serial.println("Changing mode -----------------");
    Serial.println(collection_mode);
    #endif
    collection_mode = !collection_mode;
    state_change = true;
   }
}
