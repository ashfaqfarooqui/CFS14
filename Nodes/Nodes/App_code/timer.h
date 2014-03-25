#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f4xx_tim.h"
#include "discoveryf4utils.h"
#include "state.h"


void init_Timer(void);
void init_counter(void);
void init_pwm_config(void);

#endif
