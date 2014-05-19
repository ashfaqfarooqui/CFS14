#ifndef __ACTUATOR_H_
#define __ACTUATOR_H_

#include "state.h"
#include "timer.h"
#include "gearshifting.h"

void init_actuators(void);
void ActuateShiftUp(uint8_t dutyCycle);
void actuate(GPIO_TypeDef* port,uint16_t system);
void release(GPIO_TypeDef* port,uint16_t system);

#endif
