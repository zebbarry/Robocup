//************************************
//         sensors.h     
//************************************

#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>
#include <CircularBuffer.h>

#define IR_SHORT_LEFT_PIN   A0
#define IR_SHORT_RIGHT_PIN  A1
#define IR_SHORT_FRONT_PIN  A2
#define IR_BUF_SIZE         30


enum dir {LEFT = 0, RIGHT, FRONT, NUM_IR_SENS};
enum ir_type {SHORT = 0, MEDIUM, LONG};

typedef  struct {
  CircularBuffer<int, IR_BUF_SIZE> left;
  CircularBuffer<int, IR_BUF_SIZE> right;
  CircularBuffer<int, IR_BUF_SIZE> front;
} ir_array_t;

typedef  struct {
  int left;
  int right;
  int front;
} ir_averages_t;

extern ir_averages_t ir_averages;

void infrared_init(void);

int average_buf(CircularBuffer<int, IR_BUF_SIZE>* buf);

// Convert analog value to mm
int convert_ir_dist(int analog, enum ir_type type);

// Read ultrasonic value
void send_ultrasonic(/* Parameters */);

// Read infrared value
void read_infrared(void);

// Pass in data and average the lot
void sensor_average(void);

#endif /* SENSORS_H_ */
