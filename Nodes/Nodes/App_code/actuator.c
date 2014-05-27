/*
 * actuator.c
 *
 *  Created on: May 16, 2014
 *      Author: Zeyang Geng
 */
#include "actuator.h"


void init_actuators()
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	uint16_t PrescalerValue = 0;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = CUT_IGNITION | SHIFT_DOWN|SHIFT_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = CLUTCH;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = SHIFT_UP;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect TIM8 pins  */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
	/* Compute the prescaler value */
	//SystemCoreClock = 42MHz, PrescalerValue = 1MHz
	PrescalerValue = (uint16_t)((SystemCoreClock / 2) / 21000000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10000;//1NEUTRAL_FREQUENCY;//frequency
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	

}

void ActuateShiftUp(uint8_t dutyCycle) //0-100
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (dutyCycle * 10000) / 100;//frequency
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM8, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

	/* TIM3 enable counter */
	TIM_Cmd(TIM8, ENABLE);
}

void actuate(GPIO_TypeDef* port, uint16_t system)
{
	GPIO_SetBits(port, system);
}

void release(GPIO_TypeDef* port, uint16_t system)
{
	GPIO_ResetBits(port, system);
}
