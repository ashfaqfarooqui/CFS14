#ifndef __SAFETY_H_
#define __SAFETY_H_

#include "can.h"
#include "driver_interface.h"
#include "state.h"

#define MAX 100
#define OIL 0x01
#define WATER 0x10



void safetyCheck(void);
void shutDownEngine(void);
void setFanSpeed( uint16_t);
void setCoolantPumpSpeed(uint16_t );
void coolingControl();
bol isEngineSafe();
#endif
