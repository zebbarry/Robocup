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


void led_write(int red_pin, int green_pin, int blue_pin) {
  #if DEBUG
  Serial.print("Set led state (RGB) ");
  Serial.print(red_pin);
  Serial.print(" ");
  Serial.print(green_pin);
  Serial.print(" ");
  Serial.println(blue_pin);
  #endif
  
  if (red_pin) {
    digitalWrite(red_pin, HIGH);
  } else {
    digitalWrite(red_pin, LOW);
  }

  if (green_pin) {
    digitalWrite(green_pin, HIGH);
  } else {
    digitalWrite(green_pin, LOW);
  }

  if (blue_pin) {
    digitalWrite(blue_pin, HIGH);
  } else {
    digitalWrite(blue_pin, LOW);
  }
}
