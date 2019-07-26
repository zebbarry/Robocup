
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

//**********************************************************************************
// Local Definitions
//**********************************************************************************

// Task period Definitions
// ALL OF THESE VALUES WILL NEED TO BE SET TO SOMETHING USEFUL !!!!!!!!!!!!!!!!!!!!
#define US_SEND_TASK_PERIOD                 100
#define IR_READ_TASK_PERIOD                 10   //
#define SENSOR_AVERAGE_PERIOD               100   //
#define SET_MOTOR_TASK_PERIOD               100   //
#define START_ROBOT_TASK_PERIOD             100   //
#define WEIGHT_SCAN_TASK_PERIOD             40
#define COLLECT_WEIGHT_TASK_PERIOD          40
#define CHECK_WATCHDOG_TASK_PERIOD          40
#define VICTORY_DANCE_TASK_PERIOD           40

// Task execution amount definitions
// -1 means indefinitely
#define US_SEND_TASK_NUM_EXECUTE           -1
#define IR_READ_TASK_NUM_EXECUTE           -1
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

Servo right_motor;
Servo left_motor;
ir_averages_t ir_averages;
int motor_speed_l;
int motor_speed_r;

//**********************************************************************************
// Task Scheduler and Tasks
//**********************************************************************************

/* The first value is the period, second is how many times it executes
   (-1 means indefinitely), third one is the callback function */

// Tasks for reading sensors 
Task tSend_ultrasonic(US_SEND_TASK_PERIOD,       US_SEND_TASK_NUM_EXECUTE,        &send_ultrasonic);
Task tRead_infrared(IR_READ_TASK_PERIOD,         IR_READ_TASK_NUM_EXECUTE,        &read_infrared);
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
//Task tVictory_dance(VICTORY_DANCE_TASK_PERIOD,   VICTORY_DANCE_TASK_NUM_EXECUTE,  &victory_dance);

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
    
    Serial.println("Pins have been initialised \n"); 

    pinMode(IO_POWER, OUTPUT);              //Pin 49 is used to enable IO power
    digitalWrite(IO_POWER, 1);              //Enable IO power on main CPU board

    // Initialise left and right drive motor pins
    motor_init(right_motor, RIGHT_MOTOR_PIN);
    motor_init(left_motor, LEFT_MOTOR_PIN);

    sensor_init();
}

//**********************************************************************************
// Set default robot state
//**********************************************************************************
void robot_init() {
    Serial.println("Robot is ready \n");
    motor_speed_l = MAX_SPEED;
    motor_speed_r = MAX_SPEED;
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
  taskManager.addTask(tSensor_average);
  taskManager.addTask(tSet_motor);
  taskManager.addTask(tStart_robot);
  taskManager.addTask(tWeight_scan);
  taskManager.addTask(tCollect_weight);

//  taskManager.addTask(tCheck_watchdog);
//  taskManager.addTask(tVictory_dance);      

  // Enable the tasks
  tSend_ultrasonic.enable();
  tRead_infrared.enable();
  tSensor_average.enable();
  tSet_motor.enable();
  tStart_robot.enable();
//  tWeight_scan.enable();
//  tCollect_weight.enable();
//  tCheck_watchdog.enable();
//  tVictory_dance.enable();

 Serial.println("Tasks have been initialised \n");
}



//**********************************************************************************
// put your main code here, to run repeatedly
//**********************************************************************************
void loop() {
  taskManager.execute();    //execute the scheduler
}
