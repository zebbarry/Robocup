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
typedef BNO::sAxisAnalog_t    sAxis_t;
typedef BNO::sEulAnalog_t     sEul_t;
typedef BNO::sQuaAnalog_t     sQua_t;
enum moment_t {HEAD = 0, ROLL, PITCH};
enum axis_t   {ACC = 0, MAG, GYR, LIA, GRV};


// show last sensor operate status
void printLastOperateStatus(BNO::eStatus_t eStatus);

BNO::eStatus_t imu_init(void);

sAxis_t read_imu_axis(enum axis_t axis);

sEul_t read_imu_eul(void);

sQua_t read_imu_qua(void);

// HEAD = z-axis | ROLL = y-axis | PITCH = x-axis
// RHR for direction
float read_imu_eul_dir(enum moment_t moment);

#endif /* IMU_H_ */
