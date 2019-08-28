//************************************
//         start_robot.h    
//************************************


#ifndef START_ROBOT_H_
#define START_ROBOT_H_

#define LEFT_LIMIT  200 // mm
#define RIGHT_LIMIT 200
#define FRONT_LIMIT 200
#define DESIRED_LIM 300
#define STEP        150
#define FORWARD_FULL 1800
#define FORWARD_SLOW 1700
#define BACK_FULL    1200
#define BACK_SLOW    1300
#define DUTYSCALER   1000


// Start robot sequence
void start_robot(void);

void wall_follow(void);

int PID_control(int value, int desired, float Kp, float Ki, float Kd, int current_speed);

#endif /* START_ROBOT_H_ */
