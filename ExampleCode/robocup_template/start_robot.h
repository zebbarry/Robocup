//************************************
//         start_robot.h    
//************************************


#ifndef START_ROBOT_H_
#define START_ROBOT_H_

#define LEFT_LIMIT  300 // mm
#define RIGHT_LIMIT 300
#define FRONT_LIMIT 250
#define DESIRED_LIM 300
#define STEP        15
#define FORWARD_FULL 1800
#define FORWARD_SLOW 1700
#define BACK_FULL    1100
#define BACK_SLOW    1300
#define DUTYSCALER   1000


// Start robot sequence
void start_robot(void);

int PID_control(int value, int desired, float Kp, float Ki, float Kd, int current_speed);

#endif /* START_ROBOT_H_ */
