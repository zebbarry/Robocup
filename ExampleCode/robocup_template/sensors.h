//************************************
//         sensors.h     
//************************************

#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>
#include <CircularBuffer.h>

#define IR_SHORT_LEFT_PIN   A0
#define IR_SHORT_RIGHT_PIN  A1
#define IR_SHORT_FRLT_PIN   A2
#define IR_SHORT_FRRT_PIN   A3
#define IR_BUF_SIZE         10

#define INDUCTIVE_PIN       45
#define US_TRIG_PIN         10
#define LIMIT_PIN 36


enum dir {LEFT = 0, RIGHT, FRLT, FRRT, NUM_IR_SENS};
enum ir_type {SHORT = 0, MEDIUM, LONG};

typedef  struct {
  CircularBuffer<int, IR_BUF_SIZE> left;
  CircularBuffer<int, IR_BUF_SIZE> right;
  CircularBuffer<int, IR_BUF_SIZE> frlt;
  CircularBuffer<int, IR_BUF_SIZE> frrt;
} ir_array_t;

typedef  struct {
  int left;
  int right;
  int frlt;
  int frrt;
} ir_averages_t;

extern ir_averages_t ir_averages;

extern bool collection_complete;
extern bool collection_mode;
extern bool state_change;

void sensor_init(void);

int average_buf(CircularBuffer<int, IR_BUF_SIZE>* buf);

// Convert analog value to mm
int convert_ir_dist(int analog, enum ir_type type);

// Read ultrasonic value
void send_ultrasonic(void);

// Read infrared value
void read_infrared(void);

// Pass in data and average the lot
void sensor_average(void);

#endif /* SENSORS_H_ */
