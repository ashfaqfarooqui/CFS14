#ifndef __MEGASQUIRT_H_
#define __MEGASQUIRT_H_
#include "stm32f4xx_can.h"
#include "stm32f4xx.h"
#include "state.h"
#include "actuator.h"

#define SET_RPM 9000

void launchControl();

#endif
