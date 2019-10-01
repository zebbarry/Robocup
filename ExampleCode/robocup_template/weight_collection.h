
/************************************
 *        weight_collection.h       *
 *************************************/

 /* This header is for functions and tasks for
  *  finding and collecting weights  */

#ifndef WEIGHT_COLLECTION_H_
#define WEIGHT_COLLECTION_H_

#include <Servo.h>                  //control the DC motors
#include <Wire.h>                   //for I2C and SPI


#include "sensors.h"               //will need sensor library to detect weights

#define VER_STEPS     1800  //3500
#define HOR_STEPS     800   //1000
#define MAX_ATTEMPTS  3
#define MAX_TIME      10 * 1000
#define CALIB_STEPS   1
#define HOR_RETURN    50
#define VER_RETURN    90
#define FAILED_DELAY  1000


void gantry_init(void);


void weight_scan(void);


void collect_weight(void);


void victory_dance(void);



#endif /* WEIGHT_COLLECTION_H_ */
