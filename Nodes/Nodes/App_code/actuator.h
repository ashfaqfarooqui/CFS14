#ifndef __ACTUATOR_H_
#define __ACTUATOR_H_

#include "state.h"
#include "timer.h"
#include "gearshifting.h"

#define CUT_IGNITION_PWM GPIO_Pin_1 //PB1
#define CUT_IGNITION_PWM_PINSOURCE GPIO_PinSource1
#define PERIOD_CUT_IGNITION 5000

void init_actuators(void);
void ActuateShiftUp(uint16_t dutyCycle);
void ActuateShiftDown(uint16_t dutyCycle);
void actuate(GPIO_TypeDef* port,uint16_t system);
void release(GPIO_TypeDef* port,uint16_t system);
void initPWMCutIgnition();
void cutIgnition(uint16_t );

#endif
