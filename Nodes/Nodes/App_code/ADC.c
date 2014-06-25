#include "ADC.h"


int rawAnalogState[NUMBER_OF_ANALOG_IN_PER_NODE];


/* Private variables ---------------------------------------------------------*/
__IO uint16_t RAW_ADC[80];
uint16_t ADCValues[8];

void init_ADC(void)
{
	GPIO_InitTypeDef GPIO_AdcInit;
	ADC_InitTypeDef ADC_InitStructure;

	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* Deinitializes the ADC peripheral registers */
	ADC_DeInit();

	/* Enable the GPIOA GPIOC DMA Clock & ADC1 Periph Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* DMA2 Stream0 channel0 configuration **************************************/
	DMA_Config();

	/* Configure the     PA0 pin (ADC123_IN0)
	 *                 PA3 pin (ADC123_IN3)
	 *                 PA4 pin (ADC12_IN4)
	 *                 PA5 pin (ADC12_IN5)
	 *                 PA2 pin (ADC123_in2)
	 *                 PA1 pin (ADC123_in1) */
	GPIO_AdcInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
			| GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_AdcInit.GPIO_Mode = GPIO_Mode_AN;
	GPIO_AdcInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_AdcInit);

	/* Configure the     PC4 pin (ADC12_IN14)
	 *                 PC5 pin (ADC12_IN15) */
	GPIO_AdcInit.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_AdcInit.GPIO_Mode = GPIO_Mode_AN;
	GPIO_AdcInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_AdcInit);
	/* ADC Common configuration -- ADC_CCR Register */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 regular channel 6 configuration -- ADC_CR1, ADC_CR2, ADC_SQR1 Register */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 8;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel6 configuration -- ADCx->SMPR1,SMPR2 et ADCx->SQR1,SQR2,SQR3  */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 3, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 4, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 5, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 6, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 7, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 8, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer (single-ADC mode)  */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* Enable ADC1 -- ADC_CR2_ADON */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable DMA2 Stream0 Transfer complete interrupt */
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

	/* Enable ADC1 DMA since ADC1 is the Master*/
	ADC_DMACmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);

}

void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_Config_adc();
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & RAW_ADC;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC_DR_ADDRESS;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 80;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	/* DMA2_Stream0 enable */
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

uint16_t DMA_GetADC(uint16_t Number)
{
	return ADCValues[Number];
}
void NVIC_Config_adc(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the DMA2 Stream0 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

int processAdcSamples(uint8_t index)
{
	long rawData=0;
	uint8_t sample;
	for(sample=index;sample<80;sample+=8)
	{
		rawData+=RAW_ADC[sample];
		
	}
	return rawData/10;
}

void DMA2_Stream0_IRQHandler()
{
	
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	{
		uint8_t i;
		for (i = 0; i < 8; i++)
		{
			*(rawAnalogState+i)=processAdcSamples(i);
		}
		DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
	}
	
}

void saveRawADCData()
{
	if (THIS_NODE == FRONT_NODE)
	{
		sensorData[DAMPER_TRAVEL_FL] = rawAnalogState[AN_DAMPER_TRAVEL_FL];
		sensorData[DAMPER_TRAVEL_FR] = rawAnalogState[AN_DAMPER_TRAVEL_FR];
		sensorData[BRAKE_DISC_TEMP] = rawAnalogState[AN_BRAKE_DISC_TEMP];
		sensorData[BRAKE_PRESSURE_F] = rawAnalogState[AN_BRAKE_PRESSURE_F];
		sensorData[BRAKE_PRESSURE_R] = rawAnalogState[AN_BRAKE_PRESSURE_R];
		sensorData[STEERING_ANGLE] = rawAnalogState[AN_STEERING_ANGLE];

	} else if (THIS_NODE == REAR_NODE)
	{
		sensorData[DAMPER_TRAVEL_RL] = rawAnalogState[AN_DAMPER_TRAVEL_RL];
		sensorData[DAMPER_TRAVEL_RR] = rawAnalogState[AN_DAMPER_TRAVEL_RR];
		sensorData[GEAR_POSITION] = rawAnalogState[AN_GEAR_POSITION];
		sensorData[OIL_PRESSURE] = rawAnalogState[AN_OIL_PRESSURE];
		sensorData[OIL_TEMPRATURE] = rawAnalogState[AN_OIL_TEMPRATURE];
		//sensorData[CYLINDER_POSITION] = rawAnalogState[AN_CYLINDER_POSITION];
	//	sensorData[THROTTLE_POSITION]=rawAnalogState[AN_THROTTLE_POSITION];
		sensorData[WATER_TEMPRATURE]=rawAnalogState[AN_WATER_TEMP];
	}
}



float convertData(uint8_t val)
{
	return (float)(((1.1407*(float)(val))/1000)-0.0312);
//	return (float)((((val)*3.3)/4095)*1.5);
}
