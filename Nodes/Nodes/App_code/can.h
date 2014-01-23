#ifndef __can_H_
#define __can_H_
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
//#include "stm32f4xx_can.c"
typedef enum {FAILED = 'a', PASSED = 'z'} TestStatus;

  
TestStatus transmit_data();
void init_CAN_Communication();

#endif