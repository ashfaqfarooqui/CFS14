
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
void TimingDelay_Decrement(void);
void createTaskDAQ(void);

void vCANReceiver(void *pvParameters);
void vSend50HzData(void *pvParameters);
void vSend50HzData1(void *pvParameters);
void vSend40HzData(void *pvParameters);
void vSend10HzData(void *pvParameters);
void vSend5HzData(void *pvParameters);
void vRecieveCan(void *pvParameters);
void vUpdateSensors(void *pvParameters);

void vSendWheelSpeed(void *pvParameters);
void vSendDamperTravel(void *pvParameters);
void vSendAcc(void *pvParameters);
void vSendGyro(void *pvParameters);
void vSendBrakeDisc(void *pvParameters);
void vSendSteeringAngle(void *pvParameters);
void vSendOilPressure(void *pvParameters);
void vSendWaterTemp(void *pvParameters);
void vSendGear(void *pvParameters);
     
#define STACK_SIZE_MIN	128	/* usStackDepth	- the stack size DEFINED IN WORDS.*/


GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef USART_InitStruct;

#endif
