#ifndef __GEARSHIFTING_H_
#define __GEARSHIFTING_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "discoveryf4utils.h"

#include "can.h"
#include "ADC.h"
#include "driver_interface.h"

#define MONITOR_TIME 500

#define MAX_GEAR_POSITION 3
#define MIN_GEAR_POSITION 1

/*
#define NEUTRAL 0x90//0xE7
#define RANGE_NEUTRAL 31
#define GEAR1 0x393//0x2C2//0xB3//0xD7//0xE4//0x2B0//0xA0
#define GEAR2 0xD7//0x11C//0x90//0x12E
#define GEAR3 0x162//0xB1//0x1BB
#define GEAR4 0xF8
#define GEAR5 0x1DE
#define GEAR6 0x248

#define RANGE 0x0A

#define NATURAL_CYLINDER_POSITION
*/

#define NEUTRALLOW (NEUTRAL - RANGE_NEUTRAL)
#define NEUTRALHIGH (NEUTRAL + RANGE_NEUTRAL)
#define GEAR1LOW (GEAR1 - RANGE)
#define GEAR1HIGH (GEAR1 + RANGE)
#define GEAR2LOW (GEAR2 - RANGE)
#define GEAR2HIGH (GEAR2 + RANGE)
#define GEAR3LOW (GEAR3 - RANGE)
#define GEAR3HIGH (GEAR3 + RANGE)
#define GEAR4LOW (GEAR4 - RANGE)
#define GEAR4HIGH (GEAR4 + RANGE)
#define GEAR5LOW (GEAR5 - RANGE)
#define GEAR5HIGH (GEAR5 + RANGE)
#define GEAR6LOW (GEAR6 - RANGE)
#define GEAR6HIGH (GEAR6 + RANGE)

void ShiftUpActive(void);
void ShiftDownActive(void);
void GoToNeutralActive(void);
bool GearIsInPosition(void);
Int GetGearPosition(void);
void ShiftUp(void);
void ShiftDown(void);
void GoToNeutral(void);
void CutIgnition(void);
void RestoreIgnition(void);
void ElClutch(void);
uint16_t GetGearCylinderPosition(void);
uint16_t GetGearPositionSensorData(void);

