#include <Servo.h>                  //control the DC motors
#include <stdbool.h>
#include <stdint.h>
#include "motors.h"
#include "Arduino.h"
#include "debug.h"

void motor_init(Servo motor, int pin) {
  motor.attach(pin);
  int velocity = check_speed_limits(STOP_SPEED);
  motor.writeMicroseconds(velocity);
  
  #if DEBUG
  Serial.println("Initialise motor");
  Serial.print("Change the motor speed to ");
  Serial.println(velocity);
  #endif
}


/* Check whether the speed value to be written is within the maximum
 *  and minimum speed caps. Act accordingly.
 *
 */
int check_speed_limits(int velocity) {
  if (velocity > MAX_SPEED) {
    #if DEBUG
    Serial.print("Speed too high at ");
    Serial.print(velocity);
    Serial.print(", capped to ");
    Serial.println(MAX_SPEED);
    #endif
    
    velocity = MAX_SPEED;
  } else if (velocity < MIN_SPEED) {
    #if DEBUG
    Serial.print("Speed too low at ");
    Serial.print(velocity);
    Serial.print(", capped to ");
    Serial.println(MIN_SPEED);
    #endif
    
    velocity = MIN_SPEED;
  }

  return velocity;
}


/* In this section, the motor speeds should be updated/written.
 *It is also a good idea to check whether value to write is valid.
 *It is also a good idea to do so atomically!
 */
void set_motor(void) {
  motor_speed_l = check_speed_limits(motor_speed_l);
  motor_speed_r = check_speed_limits(motor_speed_r);
  left_motor.writeMicroseconds(motor_speed_l);
  right_motor.writeMicroseconds(motor_speed_r);

  #if DEBUG
  Serial.println("Check the motor speed limit");
  Serial.print("Change the motor speed to (L, R) (");
  Serial.print(motor_speed_l);
  Serial.print(", ");
  Serial.print(motor_speed_r);
  Serial.println(") \n");
  #endif
}
