
#include <stdbool.h>
#include <stdint.h>
#include "stepper.h"
#include "Arduino.h"
#include "pin_map.h"

void stepper_init(int dir_pin, int step_pin) {
  #if DEBUG
  Serial.println("Initialise stepper \n");
  #endif
  
  pinMode(dir_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  digitalWrite(dir_pin, LOW);
}



void drive_step(int steps, int step_pin, int dir_pin, int dir) {
  if (dir) {
    digitalWrite(dir_pin, HIGH);  // up
  } else {
    digitalWrite(dir_pin, LOW);  // down
  }
  
  int i = 0;
  for (i = 0; i < steps; i++) {
    digitalWrite(step_pin,LOW);
    delayMicroseconds(2);
    digitalWrite(step_pin,HIGH);
    delay(1);
  }
}
