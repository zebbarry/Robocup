//************************************
//         stepper.h    
//************************************

#ifndef STEPPER_H_
#define STEPPER_H_

// SET THIS TO REAL VALUES
#define DIR_PIN   27          //Pin corresponding to direction pin
#define STEP_PIN  28         //Pin corresponding to step pin

enum stepper_dir {DOWN = 0, UP};

extern int current_pos;

void stepper_init(int dir_pin, int step_pin);
void drive_step(int steps, int step_pin, int dir_pin, int dir);

#endif /* MOTORS_H_ */
