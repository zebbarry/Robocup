/********************************************************************************
 *                               ROBOCUP TEMPLATE                              
 *        
 *  
 *  This is a template program design with modules for 
 *  different components of the robot, and a task scheduler
 *  for controlling how frequently tasks sholud run
 *  
 *  
 *  written by: Logan Chatfield, Ben Fortune, Lachlan McKenzie, Jake Campbell
 *  
 ******************************************************************************/

#include <stdint.h>
#include <Servo.h>                  //control the DC motors
//#include <Herkulex.h>             //smart servo
#include <Wire.h>                   //for I2C and SPI
#include <TaskScheduler.h>       //scheduler 

// Custom headers
#include "motors.h"
#include "sensors.h"
#include "weight_collection.h" 
#include "navigation.h"
#include "pin_map.h"
#include "stepper.h"
#include "led.h"
#include "imu.h"
#include "irposition.h"

//**********************************************************************************
// Local Definitions
//**********************************************************************************

// Task period Definitions
// ALL OF THESE VALUES WILL NEED TO BE SET TO SOMETHING USEFUL !!!!!!!!!!!!!!!!!!!!
#define US_SEND_TASK_PERIOD                 500
#define IR_READ_TASK_PERIOD                 20
#define CAM_READ_TASK_PERIOD                100
#define TOF_READ_TASK_PERIOD                100
#define IMU_READ_TASK_PERIOD                100
#define SENSOR_AVERAGE_PERIOD               100
#define SET_MOTOR_TASK_PERIOD               100
#define NAVIGATE_TASK_PERIOD                100
#define WEIGHT_SCAN_TASK_PERIOD             100
#define COLLECT_WEIGHT_TASK_PERIOD          100
#define CHECK_WATCHDOG_TASK_PERIOD          1000
#define VICTORY_DANCE_TASK_PERIOD           200

// Task execution amount definitions
// -1 means indefinitely
#define US_SEND_TASK_NUM_EXECUTE           -1
#define IR_READ_TASK_NUM_EXECUTE           -1
#define CAM_READ_TASK_NUM_EXECUTE          -1
#define TOF_READ_TASK_NUM_EXECUTE          -1
#define IMU_READ_TASK_NUM_EXECUTE          -1
#define SENSOR_AVERAGE_NUM_EXECUTE         -1
#define SET_MOTOR_TASK_NUM_EXECUTE         -1
#define NAVIGATE_TASK_NUM_EXECUTE          -1
#define WEIGHT_SCAN_TASK_NUM_EXECUTE       -1
#define COLLECT_WEIGHT_TASK_NUM_EXECUTE    -1
#define CHECK_WATCHDOG_TASK_NUM_EXECUTE    -1
#define VICTORY_DANCE_TASK_NUM_EXECUTE     -1

// Pin definitions
#define IO_POWER  49

// Serial definitions
#define BAUD_RATE 9600

// External definitons
Servo right_motor;
Servo left_motor;
ir_averages_t ir_averages;
int motor_speed_l;
int motor_speed_r;
int current_pos;
int tof_reading;
float imu_s2s;
float imu_f2b;
int cam_x;
int cam_y;
bool state_change;
int weight_count;
enum robot_s robot_state;

//**********************************************************************************
// Task Scheduler and Tasks
//**********************************************************************************

/* The first value is the period, second is how many times it executes
   (-1 means indefinitely), third one is the callback function */

// Tasks for reading sensors 
Task tSend_ultrasonic(US_SEND_TASK_PERIOD,       US_SEND_TASK_NUM_EXECUTE,        &send_ultrasonic);
Task tRead_infrared(IR_READ_TASK_PERIOD,         IR_READ_TASK_NUM_EXECUTE,        &read_infrared);
Task tRead_tof(TOF_READ_TASK_PERIOD,             TOF_READ_TASK_NUM_EXECUTE,       &read_tof);
Task tRead_cam(CAM_READ_TASK_PERIOD,             CAM_READ_TASK_NUM_EXECUTE,       &read_cam);
Task tRead_imu(IMU_READ_TASK_PERIOD,             IMU_READ_TASK_NUM_EXECUTE,       &read_imu);
Task tSensor_average(SENSOR_AVERAGE_PERIOD,      SENSOR_AVERAGE_NUM_EXECUTE,      &sensor_average);

// Task to set the motor speeds and direction
Task tSet_motor(SET_MOTOR_TASK_PERIOD,           SET_MOTOR_TASK_NUM_EXECUTE,      &set_motor);

// Task to start robot and check for input
Task tNavigate(NAVIGATE_TASK_PERIOD,             NAVIGATE_TASK_NUM_EXECUTE,       &navigate);

// Tasks to scan for weights and collection upon detection
Task tWeight_scan(WEIGHT_SCAN_TASK_PERIOD,       WEIGHT_SCAN_TASK_NUM_EXECUTE,    &weight_scan);
Task tCollect_weight(COLLECT_WEIGHT_TASK_PERIOD, COLLECT_WEIGHT_TASK_NUM_EXECUTE, &collect_weight);

// Tasks to check the 'watchdog' timer (These will need to be added in)
Task tCheck_watchdog(CHECK_WATCHDOG_TASK_PERIOD, CHECK_WATCHDOG_TASK_NUM_EXECUTE, &check_watchdog);
Task tVictory_dance(VICTORY_DANCE_TASK_PERIOD,   VICTORY_DANCE_TASK_NUM_EXECUTE,  &victory_dance);

Scheduler taskManager;

//**********************************************************************************
// Function Definitions
//**********************************************************************************
void pin_init();
void robot_init();
void task_init();

//**********************************************************************************
// put your setup code here, to run once:
//**********************************************************************************
void setup() {
  Serial.begin(BAUD_RATE);
  pin_init();
  robot_init();
  task_init();
  Wire.begin();
}

//**********************************************************************************
// Initialise the pins as inputs and outputs (otherwise, they won't work) 
// Set as high or low
//**********************************************************************************
void pin_init(){
  pinMode(IO_POWER, OUTPUT);              //Pin 49 is used to enable IO power
  digitalWrite(IO_POWER, 1);              //Enable IO power on main CPU board
  
  // Initialise left and right drive motor pins
  motor_init(right_motor, RIGHT_MOTOR_PIN);
  motor_init(left_motor, LEFT_MOTOR_PIN);
  stepper_init(VER_DIR_PIN, VER_STEP_PIN);
  stepper_init(HOR_DIR_PIN, HOR_STEP_PIN);
  
  pinMode(MAG_PIN, OUTPUT);
  digitalWrite(MAG_PIN, LOW);
  pinMode(LIMIT_PIN, INPUT);
  pinMode(CHAN_PIN, INPUT);
  pinMode(HOR_CALIB, INPUT);
  pinMode(VER_CALIB, INPUT);
  
  imu_init();
  cam_init();
  sensor_init();
  //    tof_init()
  gantry_init();
  reset_imu();
  
  #if DEBUG
  Serial.println("Pins have been initialised \n"); 
  #endif
}

//**********************************************************************************
// Set default robot state
//**********************************************************************************
void robot_init() {
  #if DEBUG
  Serial.println("Robot is ready \n");
  #endif
  
  motor_speed_l = STOP_SPEED;
  motor_speed_r = STOP_SPEED;
  state_change = true;
  robot_state = NO_WEIGHT;
  weight_count = 0;
}

//**********************************************************************************
// Initialise the tasks for the scheduler
//**********************************************************************************
void task_init() {  
  
  // This is a class/library function. Initialise the task scheduler
  taskManager.init();     
  
  // Add tasks to the scheduler
  taskManager.addTask(tSend_ultrasonic);   //reading ultrasonic 
  taskManager.addTask(tRead_infrared);
  taskManager.addTask(tRead_tof);
  taskManager.addTask(tRead_cam);
  taskManager.addTask(tRead_imu);
  taskManager.addTask(tSensor_average);
  taskManager.addTask(tSet_motor);
  taskManager.addTask(tNavigate);
  taskManager.addTask(tWeight_scan);
  taskManager.addTask(tCollect_weight);
  
  taskManager.addTask(tCheck_watchdog);
  taskManager.addTask(tVictory_dance);      
  
// Enable the tasks
//  tSend_ultrasonic.enable();
  tRead_infrared.enable();
//  tRead_tof.enable();
  tRead_cam.enable();
  tRead_imu.enable();
  tSensor_average.enable();
//  tSet_motor.enable();
  tNavigate.enable();
  tWeight_scan.enable();
  tCollect_weight.enable();
  tCheck_watchdog.enable();
//  tVictory_dance.enable();
  
  
  #if DEBUG 
  Serial.println("Tasks have been initialised \n");
  #endif
}



//**********************************************************************************
// put your main code here, to run repeatedly
//**********************************************************************************
void loop() {
  
  if (state_change && robot_state == WEIGHT_FOUND) {
    tCollect_weight.enable();
    tCheck_watchdog.disable();
    state_change = false;
    led_set(RED, GREEN, BLUE, false, true, false);  // Turn on Green
  } else if (state_change && robot_state == NO_WEIGHT) {
    tCollect_weight.disable();
    tCheck_watchdog.disable();
    state_change = false;
    led_set(RED, GREEN, BLUE, false, false, true);  // Turn on Blue
  } else if (state_change && robot_state == WEIGHT_AHEAD) {
    tCollect_weight.enable();
    tCheck_watchdog.enable();
    state_change = false;
    led_set(RED, GREEN, BLUE, true, false, false);  // Turn on Red
  } else if (robot_state == COMP_OVER) {
    if (state_change) {
      tWeight_scan.disable();
      tCollect_weight.disable();
      tCheck_watchdog.disable();
      led_set(RED, GREEN, BLUE, false, false, false);  // Turn on Red
    }
    led_toggle(GREEN);
  }
  
  taskManager.execute();    // Execute the scheduler
}
