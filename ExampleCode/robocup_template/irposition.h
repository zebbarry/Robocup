//************************************
//         irposition.h     
//************************************

// This file contains functions used to operate the IR positioning camera.

#include <stdbool.h>
#include <stdint.h>
#include "Arduino.h"
#include <Wire.h>

extern int cam_x;
extern int cam_y;

void Write_2bytes(byte d1, byte d2);

void cam_init(void);

void read_cam(void);
