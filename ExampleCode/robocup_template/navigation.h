//************************************
//         navigation.h    
//************************************


#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#define LEFT_LIMIT  200 // mm
#define RIGHT_LIMIT 200
#define FRONT_LIMIT 150
#define STEP        150
#define FORWARD_FULL 2000
#define FORWARD_SLOW 1700
#define BACK_FULL    1200
#define BACK_SLOW    1300
#define DESIRED_POS  1023 / 2
#define ERROR_MARG   125
#define KP           0.6
#define KI           0
#define KD           0.005
#define PID_HOLD     STOP_SPEED
#define MAX_WAIT     5
#define BLOCKED_DELAY 100


// Navigation sequence
void navigate(void);

bool obstacle_avoid(void);

void wall_follow(void);

void weight_follow(void);

int calc_weight_error(void);

int PID_control(int error, float Kp, float Ki, float Kd, bool at_max);

void check_watchdog(void);

#endif /* NAVIGATION_H_ */
