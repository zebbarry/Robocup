/*
This sample code is for testing the 4 stepper motors
All motors will rotate the same direction at the same time
*/

#include "pin_map.h"

enum ver_t {UP = 0, DOWN};
enum hor_t {LEFT = 0, RIGHT};

void drive(int step_pin, int steps, int dir_pin, int dir)
{
  digitalWrite(dir_pin, dir);
  int j;
  for(j=0;j<=steps;j++)            //Move 1000 steps
  {
    digitalWrite(step_pin,LOW);
    delayMicroseconds(2);
    digitalWrite(step_pin,HIGH);
    delay(1);
  }
}

void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  
  //Setup step and direction pins for output
  pinMode(VER_DIR_PIN,OUTPUT);
  pinMode(VER_STEP_PIN,OUTPUT);
  
  pinMode(HOR_DIR_PIN,OUTPUT);
  pinMode(HOR_STEP_PIN,OUTPUT);

  digitalWrite(VER_STEP_PIN,LOW);
  digitalWrite(HOR_STEP_PIN,LOW);
  
  //Set direction for all channels
  digitalWrite(VER_DIR_PIN,LOW);
  digitalWrite(HOR_DIR_PIN,LOW);
  
  drive(VER_STEP_PIN, 1800, VER_DIR_PIN, UP);
  delay(100);
  drive(HOR_STEP_PIN, 750, HOR_DIR_PIN, RIGHT);
//  delay(100);
//  drive(VER_STEP_PIN, 1000, VER_DIR_PIN, DOWN);
//  delay(100);
//  drive(VER_STEP_PIN, 1000, VER_DIR_PIN, UP);
//  delay(100);
//  drive(HOR_STEP_PIN, 1000, HOR_DIR_PIN, LEFT);
//  delay(100);
//  drive(VER_STEP_PIN, 2000, VER_DIR_PIN, DOWN);
}

void loop()
{
  int j;
}
