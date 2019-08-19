//************************************
//         motors.h    
//************************************

#ifndef MOTORS_H_
#define MOTORS_H_

#include <Servo.h>                  //control the DC motors

#define MIN_SPEED 1000             //Set the minimum speed value that can be written to the motors
#define MAX_SPEED 2000             //Set the maximum speed value that can be written to the motors
#define STOP_SPEED 1500            //Set the stop speed value that can be written to the motors

extern Servo right_motor;
extern Servo left_motor;
extern int motor_speed_l;
extern int motor_speed_r;

void motor_init(Servo motor, int pin);
int check_speed_limits(int velocity);
void set_motor(void);

#endif /* MOTORS_H_ */
