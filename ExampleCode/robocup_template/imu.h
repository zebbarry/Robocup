//************************************
//         imu.h     
//************************************

#ifndef IMU_H_
#define IMU_H_

#include <stdint.h>
#include "DFRobot_BNO055.h"
#include "Wire.h"

#define printAxisData(sAxis) \
  Serial.print(" x: "); \
  Serial.print(sAxis.x); \
  Serial.print(" y: "); \
  Serial.print(sAxis.y); \
  Serial.print(" z: "); \
  Serial.println(sAxis.z)

typedef DFRobot_BNO055_IIC    BNO;    // ******** use abbreviations instead of full names ********
typedef BNO::sAxisAnalog_t    sAxisAnalog_t;
typedef BNO::sEulAnalog_t     sEulAnalog_t;
typedef BNO::sQuaAnalog_t     sQuaAnalog_t;
enum moment_t {HEAD = 0, ROLL, PITCH};
enum axis_t   {ACC = 0, MAG, GYR, LIA, GRV};


// show last sensor operate status
void printLastOperateStatus(BNO::eStatus_t eStatus);

BNO::eStatus_t imu_init(void);

sAxisAnalog_t read_imu_axis(enum axis_t axis);

sEulAnalog_t read_imu_eul(void);

// HEAD = z-axis --- ROLL = y-axis --- PITCH = x-axis
// RHR for direction
float read_imu_eul_dir(enum moment_t moment);

sQuaAnalog_t read_imu_qua(void);

#endif /* IMU_H_ */
