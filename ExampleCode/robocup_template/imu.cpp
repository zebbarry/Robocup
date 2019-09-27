//************************************
//         imu.cpp       
//************************************

 // This file contains functions to use the imu sensor


#include <stdint.h>
#include "Arduino.h"
#include "DFRobot_BNO055.h"
#include "Wire.h"
#include "imu.h"
#include "pin_map.h"
#include "led.h"


BNO   bno(&Wire, 0x28);    // input TwoWire interface and IIC address


// show last sensor operate status
void printLastOperateStatus(BNO::eStatus_t eStatus)
{
  switch(eStatus) {
  case BNO::eStatusOK:   Serial.println("Everything ok"); break;
  case BNO::eStatusErr:  Serial.println("Unknown error"); break;
  case BNO::eStatusErrDeviceNotDetect:   Serial.println("Device not detected"); break;
  case BNO::eStatusErrDeviceReadyTimeOut:    Serial.println("Device ready time out"); break;
  case BNO::eStatusErrDeviceStatus:    Serial.println("Device internal status error"); break;
  default: Serial.println("Unknown status"); break;
  }
}


BNO::eStatus_t imu_init(void) {
  #if DEBUG
  Serial.println("Initialise IMU sensor \n");
  #endif
  
  bno.reset();
  BNO::eStatus_t result = bno.begin();
  
  #if DEBUG
  if (result != BNO::eStatusOK) {
    Serial.println("BNO begin failed");
    printLastOperateStatus(bno.lastOperateStatus);
  } else {
    Serial.println("BNO begin success");
  }
  Serial.println("");
  #endif

  return result;
}


sAxis_t read_imu_axis(enum axis_t axis) {
  sAxis_t result;

  switch (axis) {
    case ACC: result = bno.getAxis(BNO::eAxisAcc); break;
    case MAG: result = bno.getAxis(BNO::eAxisMag); break;
    case GYR: result = bno.getAxis(BNO::eAxisGyr); break;
    case LIA: result = bno.getAxis(BNO::eAxisLia); break;
    case GRV: result = bno.getAxis(BNO::eAxisGrv); break;
  }
  return result;
}


sEul_t read_imu_eul(void) {
  return bno.getEul();
}


sQua_t read_imu_qua(void) {
  return bno.getQua();
}


float read_imu_eul_dir(enum moment_t moment) {
  float result;
  sEul_t sEulAnalog = bno.getEul();

  switch (moment) {
    case HEAD: result = sEulAnalog.head; break;
    case ROLL: result = sEulAnalog.roll; break;
    case PITCH: result = sEulAnalog.pitch; break;
  }
  return result;
}
