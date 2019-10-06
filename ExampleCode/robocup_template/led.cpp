//************************************
//         led.cpp       
//************************************

 // This file contains functions used to turn on and off the led


#include <stdint.h>
#include "Arduino.h"
#include "pin_map.h"
#include "led.h"
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS            16
#define BRIGHTNESS          80

// Define the array of leds
CRGB leds_l[NUM_LEDS];
CRGB leds_r[NUM_LEDS];

void led_init(void) {
  #if DEBUG
  Serial.println("Initialise LEDs \n");
  #endif
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  //  LED strip
  FastLED.addLeds<NEOPIXEL, LEFT_STRIP_PIN>(leds_l, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, RIGHT_STRIP_PIN>(leds_r, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  set_led_strip(CRGB::Black, true);
  set_led_strip(CRGB::Black, false);
}


void set_led_strip(CRGB colour, int side) {   // true is left
  if (side) {
    for (int i=0; i<NUM_LEDS; i++) {
      // Turn the LED on, then pause
      leds_l[i] = colour;
    }
  } else {
    for (int i=0; i<NUM_LEDS; i++) {
      // Turn the LED on, then pause
      leds_r[i] = colour;
    }
  }
  FastLED.show();
}


void police(void) {
  static bool state = false;
  state = !state;

  if (state) {
    set_led_strip(CRGB::Red, false);
    set_led_strip(CRGB::Blue, true);
  } else {
    set_led_strip(CRGB::Blue, false);
    set_led_strip(CRGB::Red, true);
  }
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
