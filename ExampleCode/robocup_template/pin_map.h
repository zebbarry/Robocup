//************************************
//         pin_map.h     
//************************************

#ifndef PIN_MAP_H_
#define PIN_MAP_H_

// Debug mode -- 1 if compile serial outputs
#define DEBUG 1

// Arduino ports
// Analog
#define A1_1  A0  // Yellow
#define A1_2  A1  // Green
#define A2_1  A2
#define A2_2  A3
#define A3_1  A4
#define A3_2  A5
#define A4_1  A6
#define A4_2  A7
#define A5_1  A8
#define A5_2  A9
#define A6_1  A10
#define A6_2  A11

// Servo
#define S1_1  2  // Green
#define S1_2  3  // Red
#define S2_1  4
#define S2_2  5
#define S3_1  6
#define S3_2  7
#define S4_1  8
#define S4_2  9
#define S5_1  10
#define S5_2  11
#define S6_1  12
#define S6_2  13

// Digital Out
#define DO1   33  // Yellow
#define DO2   32  // Green
#define DO3   31  // Red
#define DO4   30  // Black
#define DO5   29
#define DO6   28
#define DO7   27
#define DO8   26
#define DO9   25
#define DO10  24
#define DO11  23
#define DO12  22

// Digital In
#define DI1   45  // Yellow
#define DI2   44  // Green
#define DI3   43  // Red
#define DI4   42  // Black
#define DI5   41
#define DI6   40
#define DI7   39
#define DI8   38
#define DI9   37
#define DI10  36
#define DI11  35
#define DI12  34

// Communications
#define C1_1  50  // Black  -- MISO
#define C1_2  51  // Red    -- MOSI
#define C1_3  52  // Green  -- SCK
#define C1_4  53  // Yellow -- SS
#define C2_1  16  // TX2
#define C2_2  17  // RX2
#define C2_3  18  // TX1
#define C2_4  19  // RX1
#define C3_1  1   // TX0
#define C3_2  0   // RX0
#define C3_3  14  // TX3
#define C3_4  15  // RX3

#endif /* PIN_MAP_H_ */
