
#ifndef __MAIN_H
#define __MAIN_H_

//******************************************************************************
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "discoveryf4utils.h"
#include "UART.h"
#include "can.h"
#include "ADC.h"
#include "driver_interface.h"
#include "timer.h"
#include "state.h"
#include "DAQ.h"
//******************************************************************************

//******************************************************************************
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "croutine.h"
//******************************************************************************

void setNode();
void createTaskDAQ(void);

void vSerialSender(void *pvParameters);
void vLedBlinkBlue(void *pvParameters);
void vLedBlinkRed(void *pvParameters);
void vLedBlinkGreen(void *pvParameters);
void vLedBlinkOrange(void *pvParameters);
void vCANReceiver(void *pvParameters);
     
#define STACK_SIZE_MIN	128	/* usStackDepth	- the stack size DEFINED IN WORDS.*/


GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef USART_InitStruct;

#endif
