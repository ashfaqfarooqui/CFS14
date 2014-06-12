#include "timer.h"
#define SAMPLES 10

/** This code will configure timer 2 so that it runs for a time period of one second whcih
 * will be used to calculate the freq for freq-sensors**/

//uint32_t Buffer1[SAMPLES] = 0;
//uint32_t Buffer2[SAMPLES] = 0;
uint8_t CaptureNumberLeft = 0, CaptureNumberRight = 0;
uint16_t counterLeft = 0, TimeL, counterRight = 0, TimeR;
void init_inputCapture(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* TIM1 channel 1 and 2 pin (PE.9 and PE.11) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Connect TIM pins to AF2 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
//timer base config
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //Control with dead zone.
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //Counter direction
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1; //Timer clock = sysclock /(TIM_Prescaler+1) = 2M
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;
	//Period = (TIM counter clock / TIM output clock) - 1 = 40Hz
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

	//timer channel configuration
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;

	TIM_ICInit(TIM1, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;

	TIM_ICInit(TIM1, &TIM_ICInitStructure);

	//config_Capture_DMA();
	//TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
	//TIM_DMACmd(TIM1, TIM_DMA_CC2, ENABLE);
	/* TIM enable counter */
	TIM_Cmd(TIM1, ENABLE);
	TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
	TIM_ClearFlag(TIM1, TIM_FLAG_CC2);

}

void calculateWheelSpeedLeft()
{
	uint8_t wheelID;
	if (TIM_GetFlagStatus(TIM1, TIM_FLAG_CC1) == SET)
	{
		TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
		if (CaptureNumberLeft == 0)
		{
			counterLeft = TIM_GetCapture1(TIM1);  //The first capture
			CaptureNumberLeft = 1;
		} else if (CaptureNumberLeft == 1)  //Treatment of second capture
		{
			if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != SET) //Treatment of two capture no overflow occurs
			{
				TimeR = TIM_GetCapture1(TIM1);
				TimeR = TimeR - counterLeft;
			} else
			{
				TIM_ClearFlag(TIM1, TIM_FLAG_Update);   //The update event
				TimeR = 0xFFFF - counterLeft + TIM_GetCapture1(TIM1) + 1; //If the calculation mode update events
			}
			CaptureNumberLeft = 0;
			if (TimeR != 0)
			{
				//freq = 2000000 / Time;  //Calculation of frequency
				wheelID = THIS_NODE == FRONT_NODE ? W_SPEED_FL : W_SPEED_RL;
				sensorData[wheelID] = TimeR;
			}
			
		}
	}

}
void calculateWheelSpeedRight()
{
	uint8_t wheelID;

	if (TIM_GetFlagStatus(TIM1, TIM_FLAG_CC2) == SET)
	{
		TIM_ClearFlag(TIM1, TIM_FLAG_CC2);
		if (CaptureNumberRight == 0)
		{
			counterRight = TIM_GetCapture2(TIM1);  //The first capture
			CaptureNumberRight = 1;
		} else if (CaptureNumberRight == 1)  //Treatment of second capture
		{
			if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != SET) //Treatment of two capture no overflow occurs
			{
				TimeL = TIM_GetCapture2(TIM1);
				TimeL = TimeL - counterRight;
			} else
			{
				TIM_ClearFlag(TIM1, TIM_FLAG_Update);   //The update event
				TimeL = 0xFFFF - counterRight + TIM_GetCapture2(TIM1) + 1; //If the calculation mode update events
			}
			CaptureNumberRight = 0;
			if (TimeL != 0)
			{
				wheelID = THIS_NODE == FRONT_NODE ? W_SPEED_FR : W_SPEED_RR;
				//freq = 2000000 / Time;  //Calculation of frequency
				sensorData[wheelID] = TimeL;
			}
		}
	}

}
/*void config_Capture_DMA()
 {
 //chanel 1

 DMA_InitTypeDef DMA_InitStructure;
 DMA_DeInit(DMA2_Stream3);

 DMA_StructInit(&DMA_InitStructure);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

 DMA_InitStructure.DMA_Channel = DMA_Channel_6;
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(TIM1->CCR1));
 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & Buffer1[0];
 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
 DMA_InitStructure.DMA_BufferSize = SAMPLES;
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 32-bit
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 DMA_Init(DMA2_Stream3, &DMA_InitStructure);
 DMA_Cmd(DMA2_Stream3, ENABLE);
 //channel 2
 DMA_DeInit(DMA2_Stream2);

 DMA_StructInit(&DMA_InitStructure);

 DMA_InitStructure.DMA_Channel = DMA_Channel_6;
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(TIM1->CCR2));
 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & Buffer2[0];
 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
 DMA_InitStructure.DMA_BufferSize = SAMPLES;
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 32-bit
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 DMA_Init(DMA2_Stream2, &DMA_InitStructure);
 DMA_Cmd(DMA2_Stream2, ENABLE);
 }*/

void delay(uint16_t delay)
{
	uint16_t tim = 0;
	TIM_TimeBaseInitTypeDef timerInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	tim = (uint16_t) delay * (42000) / 41999;

	timerInitStructure.TIM_Prescaler = 42000 - 1; //1MHz
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = tim;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	while (!(TIM_GetITStatus(TIM2, TIM_IT_Update)))
	{
		;
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_Cmd(TIM2, DISABLE);

}
/**
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
 */
/** configure the counters for each timer*/
/**
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
 */

////////////////////////////////////////////////////////////////////////////
/*PWM*/
/////////////////////////////////////////////////////////////////////////////
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
	GPIO_InitStructure.GPIO_Pin = FAN_OUT_PIN | COOLANT_PUMP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect TIM3 pins  */
	GPIO_PinAFConfig(GPIOC, FANOUT_PINSOURCE, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, COOLANT_PUMP_PINSOURCE, GPIO_AF_TIM3);
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t)((SystemCoreClock / 2) / 21000000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10000;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

}
