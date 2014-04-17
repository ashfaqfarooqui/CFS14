#include "safety.h"

void waterTempCheck()
{
	uint8_t temp = sensorData[WATER_TEMPRATURE];
	if (temp > 120)
	{
		shutDownEngine();
	} else if (temp > 110)
	{
		setFanSpeed(MAX);
		SwitchWarningLight(ON);
	} else if (temp > 95)
	{
		setFanSpeed(90);
	} else
	{
		setFanSpeed(75);
	}
}

void oilTempCheck()
{

	uint8_t temp = sensorData[OIL_PRESSURE];
	if (temp > 120)
	{
		shutDownEngine();
	}

}

void shutDownEngine()
{
	
//TODO	actuate(GPIO,pin);
}
void setFanSpeed(uint8_t dutyCycle)
{
	uint16_t pwmSpeed = (dutyCycle * 665) / 100;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pwmSpeed;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}
