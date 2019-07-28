//************************************
//         stepper.h    
//************************************

#ifndef STEPPER_H_
#define STEPPER_H_

// SET THIS TO REAL VALUES
#define DIR_PIN   27          //Pin corresponding to direction pin
#define STEP_PIN  28         //Pin corresponding to step pin

void stepper_init(int dir_pin, int step_pin);
void drive_step(int steps, int step_pin);

#endif /* MOTORS_H_ */
