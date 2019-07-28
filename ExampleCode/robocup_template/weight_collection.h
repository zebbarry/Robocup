
/************************************
 *        weight_collection.h       *
 *************************************/

 /* This header is for functions and tasks for
  *  finding and collecting weights  */

#ifndef WEIGHT_COLLECTION_H_
#define WEIGHT_COLLECTION_H_

#include <Servo.h>                  //control the DC motors
#include <Herkulex.h>               //smart servo
#include <Wire.h>                   //for I2C and SPI


#include "sensors.h"               //will need sensor library to detect weights

#define MAG_PIN   33
#define FAN_PIN   32
#define STEPS     2000

//states for swapping between searching and collecting
enum weight_s {NO_WEIGHT = 0, WEIGHT_FOUND};


void weight_scan(void);


void collect_weight(void);


void victory_dance(void);



#endif /* WEIGHT_COLLECTION_H_ */
