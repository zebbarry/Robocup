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
  Serial.println("Check the motor speed limit");
  
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
  int vel_l = check_speed_limits(motor_speed_l);
  int vel_r = check_speed_limits(motor_speed_r);
  left_motor.writeMicroseconds(vel_l);
  right_motor.writeMicroseconds(vel_r);
  
  Serial.print("Change the motor speed to (L, R) (");
  Serial.print(vel_l);
  Serial.print(", ");
  Serial.print(vel_r);
  Serial.println(") \n");
}
