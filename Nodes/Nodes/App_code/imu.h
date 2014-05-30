#ifndef __IMU_H_
#define __IMU_H_

#include "state.h"
#include "stm32f4xx_usart.h"

#define acc_x 0
#define acc_y 1
#define acc_z 2
#define gyro_yaw 3
#define gyro_pitch 4
#define gyro_roll 5

#define START_PACKET 0xF7
#define GYRO_CMD 0x26
#define ACC_CMD 0x27

void requestGyroData();
void requestAccData();
void imuManager();






#endif
