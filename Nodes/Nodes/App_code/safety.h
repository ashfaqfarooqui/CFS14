#ifndef __SAFETY_H_
#define __SAFETY_H_

#include "can.h"
#include "driver_interface.h"

#define MAX 100
#define OIL 0x01
#define WATER 0x10



void safetyCheck(void);
void shutDownEngine(void);
void setFanSpeed( uint8_t);
void setCoolantPumpSpeed(uint8_t );
void coolingControl();
#endif
