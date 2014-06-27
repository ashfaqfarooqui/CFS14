#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"
#include "discoveryf4utils.h"
#include "state.h"

#define PERIOD_FAN 100
#define PERIOD_GEAR 10000
//void init_Timer(void);
//void init_counter(void);
void init_pwm_config(void);
void calculateWheelSpeedLeft();
void calculateWheelSpeedRight();
void init_inputCapture(void);
void config_Capture_DMA(void);
void delay(uint16_t);

//timers for gearshifting
void startTimer();
void stopTimer();
uint16_t getTimerValue();

#endif
