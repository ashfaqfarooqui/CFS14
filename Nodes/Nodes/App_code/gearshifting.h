#ifndef __GEARSHIFTING_H_
#define __GEARSHIFTING_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "discoveryf4utils.h"

#include "can.h"
#include "ADC.h"
#include "driver_interface.h"
#include "state.h"

typedef int bool;
#define true 1
#define false 0
	
#define CLUTCH GPIO_Pin_15
#define SHIFT_UP GPIO_Pin_0
#define SHIFT_DOWN GPIO_Pin_1
#define CUT_IGNITION GPIO_Pin_14
#define NEUTRAL GPIO_Pin_13
#define INPOSITION GPIO_Pin_6


#define MONITOR_TIME 500
#define SWITCHHOLDINGTIME 100000
#define LAUCH_CONTROL_CLUTCH_ON_DELAY 150

#define MAX_GEAR_POSITION 3
#define MIN_GEAR_POSITION 1


#define NEUTRAL 0x0927//2.343V//0xE7
#define RANGE_NEUTRAL 10
#define GEAR1 0x07CB//1.995V//0x2C2//0xB3//0xD7//0xE4//0x2B0//0xA0
#define GEAR2 0x0A98//2.712V//0x11C//0x90//0x12E
#define GEAR3 0x0D6F//3.439V//3.0xB1//0x1BB
#define GEAR4 3814
#define GEAR5 4475
#define GEAR6 1159

#define RANGE 50

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
#define SHIFTDOWN2RPM_PROTECTION 8500
#define SHIFTDOWN3RPM_PROTECTION 10500

void gearShiftManager(void);
int GetGearPosition(void);
void ShiftUp(int);
void ShiftDown(int);
void GoToNeutral(void);
void CutIgnition(void);
void ElClutch(int);
void AutoShifting(void);
void LaunchControl(void);



#define GEAR_SHIFT_PORT GPIOE
void actuate(GPIO_TypeDef* port,uint16_t system);
void release(GPIO_TypeDef* port,uint16_t system);
void init_actuators();

#endif
