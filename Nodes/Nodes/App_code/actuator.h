#ifndef __ACTUATOR_H_
#define __ACTUATOR_H_

#include "state.h"

void actuateShiftUpSolonoid(uint8_t dutyCycle);
void actuate(GPIO_TypeDef* port,uint16_t system);
void release(GPIO_TypeDef* port,uint16_t system);

#endif
