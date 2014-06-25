#ifndef __GEARSHIFTING_H_
#define __GEARSHIFTING_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "discoveryf4utils.h"

#include "can.h"
#include "timer.h"
#include "ADC.h"
#include "driver_interface.h"
#include "state.h"
#include "actuator.h"

typedef int bool;
#define true 1
#define false 0

#define CLUTCH GPIO_Pin_9 //PA9
#define SHIFT_UP GPIO_Pin_6 //PC6
#define SHIFT_DOWN GPIO_Pin_7 //PC7
#define CUT_IGNITION GPIO_Pin_1 //PE1


#define MONITOR_TIME 300000
#define SWITCHHOLDINGTIME 100000
#define LAUCH_CONTROL_CLUTCH_ON_DELAY 150
#define NEUTRAL_FREQUENCY 10

#define MAX_GEAR_POSITION 3
#define MIN_GEAR_POSITION 1


#define NEUTRAL 0x07E7//2.343V//0xE7
#define RANGE_NEUTRAL 100
#define GEAR1 0x06BF//1.995V//0x2C2//0xB3//0xD7//0xE4//0x2B0//0xA0
#define GEAR2 0x0912//2.712V//0x11C//0x90//0x12E
#define GEAR3 0x0B6A//3.439V//3.0xB1//0x1BB
#define GEAR4 0x3814
#define GEAR5 0x4475
#define GEAR6 0x1159

#define RANGE 180
#define RANGE_FIRST_HIGH 120

#define NEUTRALLOW (NEUTRAL - RANGE_NEUTRAL)
#define NEUTRALHIGH (NEUTRAL + RANGE_NEUTRAL)
#define GEAR1LOW (GEAR1 - RANGE)
#define GEAR1HIGH (GEAR1 + RANGE_FIRST_HIGH)
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
#define SHIFTDOWN2RPM_PROTECTION 8500
#define SHIFTDOWN3RPM_PROTECTION 10500

void gearShiftManager(void);
int GetGearPosition(void);
void ShiftUp(int);
void ShiftDown(int);
void GoToNeutral(void);
void CutIgnition(void);
void ElClutch(bol);
void AutoShifting(void);
//void LaunchControl(void);
void ActiveClutch(void);
void InactiveClutch(void);
void ActiveShiftUp(void);
void InactiveShiftUp(void);
void ActiveShiftDown(void);
void InactiveShiftDown(void);
void ActiveCutIgnition(void);
void InactiveCutIgnition(void);


#endif
