//************************************
//         sensors.h     
//************************************

#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>
#include <CircularBuffer.h>

#define IR_BUF_SIZE         10

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
extern int tof_reading;
extern float imu_s2s;
extern float imu_f2b;

void sensor_init(void);

void tof_init(void);

void read_tof(void);

void read_imu(void);

int average_buf(CircularBuffer<int, IR_BUF_SIZE>* buf);

// Convert analog value to mm
int convert_ir_dist(int analog, enum ir_type type);

int convert_ir_dist_short(int analog);
int convert_ir_dist_medium(int analog);

// Read ultrasonic value
void send_ultrasonic(void);

// Read infrared value
void read_infrared(void);

// Pass in data and average the lot
void sensor_average(void);

#endif /* SENSORS_H_ */
