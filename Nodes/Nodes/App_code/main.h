
#ifndef __MAIN_H
#define __MAIN_H_

//******************************************************************************
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_IWDG.h"
#include "discoveryf4utils.h"
#include "UART.h"
#include "can.h"
#include "ADC.h"
#include "driver_interface.h"
#include "timer.h"
#include "state.h"
#include "DAQ.h"
#include "gearshifting.h"
#include "actuator.h"
#include "megasquirt.h"
#include "IMU.h"
#include "actuator.h"
#include "telematics.h"

//******************************************************************************

//******************************************************************************
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "croutine.h"
//******************************************************************************
void TimingDelay_Decrement(void);
void createTaskDAQ(void);
void initializeSystem();
void initIWDG();

void vCANReceiver(void *pvParameters);
void vSend50HzData(void *pvParameters);
void vSend50HzData1(void *pvParameters);
void vSend40HzData(void *pvParameters);
void vSend10HzData(void *pvParameters);
void vSendBreakPressureData(void *pvParameters);
void vsendCaliperTemp(void *pvParameters);
void vRecieveCan(void *pvParameters);
void vUpdateInputs(void *pvParameters);
void vPerformSwitchAction(void *pvParameters);
void vUpdateADC(void *pvParameters);
void vIWDGUpdate(void *pvParameters);

void vUpdateWheelSpeedLeft(void *pvParameters);
void vUpdateWheelSpeedRight(void *pvParameters);

void vSafetyCheck(void *pvParameters);
void vCoolingSystem(void *pvParameters);
void vGearShifting(void *pvParameters);
void vIMUManager(void *pvParameters);
void vLaunchControl(void *pvParameters);

void vSendWheelSpeed(void *pvParameters);
void vSendDamperTravel(void *pvParameters);
void vSendAcc(void *pvParameters);
void vSendGyro(void *pvParameters);
void vSendBrakeDisc(void *pvParameters);
void vSendSteeringAngle(void *pvParameters);
void vSendOilPressure(void *pvParameters);
void vSendWaterTemp(void *pvParameters);
void vSendGear(void *pvParameters);
void vSendGearDataRaw(void *pvParameters);
void vNeutralMgr(void *pvParameters);
void vautoShiftMgr(void *pvParameters);
void vCalculteBrakeBias(void *pvparameters);
void vTelematics(void *pvParameters);
     
#define STACK_SIZE_MIN	128	/* usStackDepth	- the stack size DEFINED IN WORDS.*/


GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef USART_InitStruct;

static uint32_t GetLSIFrequency(void);

#endif
