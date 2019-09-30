//************************************
//         irposition.cpp       
//************************************

 // This file contains functions used to operate the IR positioning camera.
 
// Wii Remote IR sensor  test sample code  by kako http://www.kako.com
// modified output for Wii-BlobTrack program by RobotFreak http://www.letsmakerobots.com/user/1433
// modified for http://DFRobot.com by Lumi, Jan. 2014

#include <stdbool.h>
#include <stdint.h>
#include "Arduino.h"
#include <Wire.h>
#include "irposition.h"
#include "pin_map.h"

int IRsensorAddress = 0xB0;

int slaveAddress;
byte data_buf[16];
int i;
int s;
int num_coords = 1;

void Write_2bytes(byte d1, byte d2)
{
  Wire.beginTransmission(slaveAddress);
  Wire.write(d1); Wire.write(d2);
  Wire.endTransmission();
}

void cam_init(void)
{  
  #if DEBUG
  Serial.println("Initialising IR positioning camera");
  #endif
  
  slaveAddress = IRsensorAddress >> 1;   // This results in 0x21 as the address to pass to TWI
  // IR sensor initialize
  Write_2bytes(0x30, 0x01); delay(10);
  Write_2bytes(0x30, 0x08); delay(10);
  Write_2bytes(0x06, 0x90); delay(10);
  Write_2bytes(0x08, 0xC0); delay(10);
  Write_2bytes(0x1A, 0x40); delay(10);
  Write_2bytes(0x33, 0x33); delay(10);
  delay(100);
}

void read_cam(void)
{
  int start_t = millis();
  //IR sensor read
  Wire.beginTransmission(slaveAddress);
  Wire.write(0x36);
  Wire.endTransmission();

  Wire.requestFrom(slaveAddress, 16);        // Request the 2 byte heading (MSB comes first)
  for (i = 0; i < 16; i++) {
    data_buf[i] = 0;
  }
  i = 0;
  while (Wire.available() && i < 16) {
    data_buf[i] = Wire.read();
    i++;
  }

  //Manipulate data received into useable format
  cam_x[0] = data_buf[1];
  cam_y[0] = data_buf[2];
  s   = data_buf[3];
  cam_x[0] += (s & 0x30) << 4;
  cam_y[0] += (s & 0xC0) << 2;

  cam_x[1] = data_buf[4];
  cam_y[1] = data_buf[5];
  s   = data_buf[6];
  cam_x[1] += (s & 0x30) << 4;
  cam_y[1] += (s & 0xC0) << 2;

  cam_x[2] = data_buf[7];
  cam_y[2] = data_buf[8];
  s   = data_buf[9];
  cam_x[2] += (s & 0x30) << 4;
  cam_y[2] += (s & 0xC0) << 2;

  cam_x[3] = data_buf[10];
  cam_y[3] = data_buf[11];
  s   = data_buf[12];
  cam_x[3] += (s & 0x30) << 4;
  cam_y[3] += (s & 0xC0) << 2;

  #if DEBUG
  Serial.print("Reading ir positioning camera: ");
  
  //Display data on the serial port
  for (i = 0; i < num_coords; i++)
  {
    Serial.print( int(cam_x[i]) );
    Serial.print(",");
    Serial.print( int(cam_y[i]) );
    if (i < num_coords - 1)
      Serial.print(",");
  }
  Serial.println("");
  #endif
  int end_t = millis();
  Serial.println(end_t-start_t);
}
