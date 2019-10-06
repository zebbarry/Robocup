//************************************
//         navigation.h    
//************************************


#ifndef NAVIGATION_H_
#define NAVIGATION_H_

// IR Limits
#define LEFT_LIMIT    200 // mm
#define RIGHT_LIMIT   200
#define LEFT_EXT_LIMIT    80
#define RIGHT_EXT_LIMIT   80
#define FRONT_LIMIT   150
#define TOF_LIMIT     100

// Speed constants
#define STEP          100
#define FORWARD_FULL  2000
#define FORWARD_SLOW  1800
#define APPROACHING   1700
#define BACK_FULL     1100
#define BACK_SLOW     1200

// Navigation constants
#define DESIRED_POS   1023 / 2
#define ERROR_MARG    125
#define KP            0.7
#define KI            0
#define KD            0.001
#define PID_HOLD      FORWARD_SLOW
#define MAX_WAIT      5
#define BLOCKED_DELAY 100
#define RAMP_DELAY    100
#define MAX_FRWD_ANG  8
#define MAX_SIDE_ANG  5

enum angle_s {FLAT = 0, FRWD, BACK, LEFT, RGHT, FRWD_LEFT, FRWD_RGHT, BACK_LEFT, BACK_RGHT};

// Navigation sequence
void navigate(void);

bool check_angle(float angle);

enum angle_s check_ramp(float side2side, float front2back);

bool obstacle_avoid(void);

void wall_follow(void);

void weight_follow(void);

int calc_weight_error(void);

int PID_control(int error, float Kp, float Ki, float Kd, bool at_max);

void check_watchdog(void);

#endif /* NAVIGATION_H_ */
