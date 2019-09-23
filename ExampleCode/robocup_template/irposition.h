//************************************
//         irposition.h     
//************************************

// This file contains functions used to operate the IR positioning camera.

#include "Arduino.h"
#include <Wire.h>

extern int cam_x[4];
extern int cam_y[4];

void Write_2bytes(byte d1, byte d2);

void cam_init(void);

void read_cam(void);
