//************************************
//         led.cpp       
//************************************

 // This file contains functions used to turn on and off the led


#include <stdint.h>
#include "Arduino.h"
#include "pin_map.h"
#include "led.h"


void led_init(void) {
  #if DEBUG
  Serial.println("Initialise led \n");
  #endif
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}


void led_on(int led_pin) {
  #if DEBUG
  Serial.print("Turn on led ");
  Serial.println(led_pin);
  #endif
  digitalWrite(led_pin, HIGH);
}


void led_off(int led_pin) {
  #if DEBUG
  Serial.print("Turn off led ");
  Serial.println(led_pin);
  #endif
  digitalWrite(led_pin, LOW);
}


bool led_toggle(int led_pin) {
  static bool state = false;
  state = !state;
   if (state) {
    digitalWrite(led_pin, HIGH);
  } else {
    digitalWrite(led_pin, LOW);
  }
  return state;
}


void led_set(int red_pin, int green_pin, int blue_pin, bool red, bool green, bool blue) {
  #if DEBUG
  Serial.print("Set led state (RGB) ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.println(blue);
  #endif
  
  digitalWrite(red_pin, red);
  digitalWrite(green_pin, green);
  digitalWrite(blue_pin, blue);
}
