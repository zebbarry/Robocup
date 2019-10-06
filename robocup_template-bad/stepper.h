//************************************
//         stepper.h    
//************************************

#ifndef STEPPER_H_
#define STEPPER_H_

enum stepper_ver {UP_S = 0, DOWN_S};
enum stepper_hor {LEFT_S = 0, RIGHT_S};

extern int current_pos;

void stepper_init(int dir_pin, int step_pin);

void drive_step(int steps, int step_pin, int dir_pin, int dir);

#endif /* MOTORS_H_ */
