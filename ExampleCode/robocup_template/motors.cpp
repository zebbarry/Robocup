#include <Servo.h>                  //control the DC motors
#include <stdbool.h>
#include <stdint.h>
#include "motors.h"
#include "Arduino.h"

void motor_init(Servo motor, int pin) {
  Serial.println("Initialise motor");
  motor.attach(pin);
  int velocity = check_speed_limits(STOP_SPEED);
  
  Serial.print("Change the motor speed to ");
  Serial.println(velocity);
  motor.writeMicroseconds(velocity);
}


/* Check whether the speed value to be written is within the maximum
 *  and minimum speed caps. Act accordingly.
 *
 */
int check_speed_limits(int velocity) {
  if (velocity > MAX_SPEED) {
    Serial.print("Speed too high at ");
    Serial.print(velocity);
    Serial.print(", capped to ");
    Serial.println(MAX_SPEED);
    
    velocity = MAX_SPEED;
  } else if (velocity < MIN_SPEED) {
    Serial.print("Speed too low at ");
    Serial.print(velocity);
    Serial.print(", capped to ");
    Serial.println(MIN_SPEED);
    
    velocity = MIN_SPEED;
  }

  return velocity;
}


/* In this section, the motor speeds should be updated/written.
 *It is also a good idea to check whether value to write is valid.
 *It is also a good idea to do so atomically!
 */
void set_motor(void) {
  Serial.println("Check the motor speed limit");
  motor_speed_l = check_speed_limits(motor_speed_l);
  motor_speed_r = check_speed_limits(motor_speed_r);
  left_motor.writeMicroseconds(motor_speed_l);
  right_motor.writeMicroseconds(motor_speed_r);
  
  Serial.print("Change the motor speed to (L, R) (");
  Serial.print(motor_speed_l);
  Serial.print(", ");
  Serial.print(motor_speed_r);
  Serial.println(") \n");
}



void drive_step(int steps, int step_pin) {
  int i = 0;
  for (i = 0; i < steps; i++) {
    digitalWrite(step_pin,LOW);
    delayMicroseconds(2);
    digitalWrite(step_pin,HIGH);
    delay(1);
  }
}
