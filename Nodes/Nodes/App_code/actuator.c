/*
 * actuator.c
 *
 *  Created on: May 16, 2014
 *      Author: Zeyang Geng
 */
#include "actuator.h"


void actuateShiftUpSolonoid(uint8_t dutyCycle) //0-100
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (dutyCycle * NEUTRAL_FREQUENCY) / 100;//frequency
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
