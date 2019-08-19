
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
#include "TaskScheduler\src\TaskScheduler.h"          //scheduler 

// Custom headers
#include "motors.h"
#include "sensors.h"
#include "weight_collection.h" 
#include "start_robot.h"
#include "pin_map.h"
#include "stepper.h"
#include "led.h"
#include "imu.h"

//**********************************************************************************
// Local Definitions
//**********************************************************************************

// Task period Definitions
// ALL OF THESE VALUES WILL NEED TO BE SET TO SOMETHING USEFUL !!!!!!!!!!!!!!!!!!!!
#define US_SEND_TASK_PERIOD                 500
#define IR_READ_TASK_PERIOD                 15
#define TOF_READ_TASK_PERIOD                100
#define IMU_READ_TASK_PERIOD                100
#define SENSOR_AVERAGE_PERIOD               150
#define SET_MOTOR_TASK_PERIOD               150
#define START_ROBOT_TASK_PERIOD             150
#define WEIGHT_SCAN_TASK_PERIOD             200
#define COLLECT_WEIGHT_TASK_PERIOD          200
#define CHECK_WATCHDOG_TASK_PERIOD          40
#define VICTORY_DANCE_TASK_PERIOD           200

// Task execution amount definitions
// -1 means indefinitely
#define US_SEND_TASK_NUM_EXECUTE           -1
#define IR_READ_TASK_NUM_EXECUTE           -1
#define TOF_READ_TASK_NUM_EXECUTE          -1
#define IMU_READ_TASK_NUM_EXECUTE          -1
#define SENSOR_AVERAGE_NUM_EXECUTE         -1
#define SET_MOTOR_TASK_NUM_EXECUTE         -1
#define START_ROBOT_TASK_NUM_EXECUTE       -1
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
float imu_reading;
bool collection_complete;
bool collection_mode;
bool state_change;

//**********************************************************************************
// Task Scheduler and Tasks
//**********************************************************************************

/* The first value is the period, second is how many times it executes
   (-1 means indefinitely), third one is the callback function */

// Tasks for reading sensors 
Task tSend_ultrasonic(US_SEND_TASK_PERIOD,       US_SEND_TASK_NUM_EXECUTE,        &send_ultrasonic);
Task tRead_infrared(IR_READ_TASK_PERIOD,         IR_READ_TASK_NUM_EXECUTE,        &read_infrared);
Task tRead_tof(TOF_READ_TASK_PERIOD,             TOF_READ_TASK_NUM_EXECUTE,       &read_tof);
Task tRead_imu(IMU_READ_TASK_PERIOD,             IMU_READ_TASK_NUM_EXECUTE,       &read_imu);
Task tSensor_average(SENSOR_AVERAGE_PERIOD,      SENSOR_AVERAGE_NUM_EXECUTE,      &sensor_average);

// Task to set the motor speeds and direction
Task tSet_motor(SET_MOTOR_TASK_PERIOD,           SET_MOTOR_TASK_NUM_EXECUTE,      &set_motor);

// Task to start robot and check for input
Task tStart_robot(START_ROBOT_TASK_PERIOD,       START_ROBOT_TASK_NUM_EXECUTE,    &start_robot);

// Tasks to scan for weights and collection upon detection
Task tWeight_scan(WEIGHT_SCAN_TASK_PERIOD,       WEIGHT_SCAN_TASK_NUM_EXECUTE,    &weight_scan);
Task tCollect_weight(COLLECT_WEIGHT_TASK_PERIOD, COLLECT_WEIGHT_TASK_NUM_EXECUTE, &collect_weight);

// Tasks to check the 'watchdog' timer (These will need to be added in)
//Task tCheck_watchdog(CHECK_WATCHDOG_TASK_PERIOD, CHECK_WATCHDOG_TASK_NUM_EXECUTE, &check_watchdog);
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
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);
    pinMode(LIMIT_PIN, INPUT);
    sensor_init();
//    tof_init();
//    imu_init();
    
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
    collection_complete = false;
    collection_mode = true;
    state_change = true;
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
  taskManager.addTask(tRead_imu);
  taskManager.addTask(tSensor_average);
  taskManager.addTask(tSet_motor);
  taskManager.addTask(tStart_robot);
  taskManager.addTask(tWeight_scan);
  taskManager.addTask(tCollect_weight);

//  taskManager.addTask(tCheck_watchdog);
  taskManager.addTask(tVictory_dance);      

  // Enable the tasks
  tSend_ultrasonic.enable();
  tRead_infrared.enable();
//  tRead_tof.enable();
//  tRead_imu.enable();
  tSensor_average.enable();
//  tSet_motor.enable();
//  tStart_robot.enable();
  tWeight_scan.enable();
  tCollect_weight.enable();
//  tCheck_watchdog.enable();
//  tVictory_dance.enable();


  #if DEBUG 
  Serial.println("Tasks have been initialised \n");
  #endif
}



//**********************************************************************************
// put your main code here, to run repeatedly
//**********************************************************************************
void loop() {
//  if (collection_complete) {
//    tVictory_dance.enable();
//  } else {
//    tVictory_dance.disable();
//  }

  if (collection_mode && state_change) {
    left_motor.writeMicroseconds(STOP_SPEED);
    right_motor.writeMicroseconds(STOP_SPEED);
    tSet_motor.disable();
    tStart_robot.disable();
    tWeight_scan.enable();
    tCollect_weight.enable();
    state_change = false;
    led_off(BLUE);
  } else if (state_change) {
    tSet_motor.enable();
    tStart_robot.enable();
    tWeight_scan.disable();
    tCollect_weight.disable();
    state_change = false;
    led_off(GREEN);
  }
  
  taskManager.execute();    //execute the scheduler
}
