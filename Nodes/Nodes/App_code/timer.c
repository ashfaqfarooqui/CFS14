#include "timer.h"
/** This code will configure timer 2 so that it runs for a time period of one second whcih
 * will be used to calculate the freq for freq-sensors**/

void init_Timer()
{
	TIM_TimeBaseInitTypeDef timerInitStructure;
	NVIC_InitTypeDef nvicStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	timerInitStructure.TIM_Prescaler = 42000 - 1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 2000 - 1;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);

	//initialize interupts

	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
//	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

	//initialize Interupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		STM_EVAL_LEDToggle(LED_RED);
	}
	//TODO Get reading of counter
	//TODO reset counters
	//TODO put data into raw variables for the sensors, Calculate them when required to display
}

/** configure the counters for each timer*/
void init_counter()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

}

/**
 In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1),
 since APB1 prescaler is different from 1.
 TIM3CLK = 2 * PCLK1
 PCLK1 = HCLK / 4
 => TIM3CLK = HCLK / 2 = SystemCoreClock /2

 To get TIM3 counter clock at 21 MHz, the prescaler is computed as follows:
 Prescaler = (TIM3CLK / TIM3 counter clock) - 1
 Prescaler = ((SystemCoreClock /2) /21 MHz) - 1

 To get TIM3 output clock at 30 KHz, the period (ARR)) is computed as follows:
 ARR = (TIM3 counter clock / TIM3 output clock) - 1
 = 665
 */
void init_pwm_config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	uint16_t PrescalerValue = 0;

	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOC clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* GPIOC Configuration:*/
	GPIO_InitStructure.GPIO_Pin = FAN_OUT_PIN;	//TODO configure correct pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect TIM3 pins to AF2 */
	GPIO_PinAFConfig(GPIOC, FANOUT_PINSOURCE, GPIO_AF_TIM3);//TODO configure correct pin

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t)((SystemCoreClock / 2) / 21000000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 665;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

}
