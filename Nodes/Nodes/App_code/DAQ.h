#ifndef __DAQ_H_
#define __DAQ_H_

#include "state.h"
#include "can.h"


#define CAN_ADR_DAMPER_POSITION	0x101
#define CAN_ADR_BRAKE_PRESSURE 0x112
#define CAN_ADR_OIL_PRESSURE 0x113
#define CAN_ADR_OIL_TEMPRATURE 0x142
#define CAN_ADR_WATER_TEMPRATURE 0x140
#define CAN_ADR_SHIFTING 0x146
#define CAN_ADR_WHEEL_SPEED	0x120
#define CAN_ADR_STEERING_WHEEL_ANGLE 0x121
#define CAN_ADR_READ_WHEEL_SLIP 0x123
#define CAN_ADR_ACC 0x125
#define CAN_ADR_GYRO 0x133
#define CAN_ADR_BRAKE_DISC_TEMP 0x134
#define CAN_ADR_LAMBDA 0x148
#define CAN_ADR_THROTTLEPOSTION 0x150
#define CAN_ADR_MAP 0x144
#define CAN_ADR_GEAR 0x146
#define CAN_ADR_TIRE_TEMP 0x136


#define CAN_FRONT_NODE_FILTER 64
#define CAN_REAR_NODE_FILTER 32


#if THIS_NODE==FRONT_NODE
#define CAN_NODE_FILTER CAN_FRONT_NODE_FILTER
#elif THIS_NODE==REAR_NODE
#define CAN_NODE_FILTER CAN_REAR_NODE_FILTER
#endif




void sendWheelSpeed(void);
void sendDamperPosition(void);
void sendBrakePressure(void);
void sendOilPressure(void);



#endif
