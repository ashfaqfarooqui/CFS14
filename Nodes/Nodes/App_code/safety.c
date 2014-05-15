#include "safety.h"

void safetyCheck()
{
	uint8_t temp = sensorData[WATER_TEMPRATURE];
	uint8_t checkLight=1;
	if (temp > 120)
	{
		shutDownEngine();

	} else if (temp > 110)
	{
		setFanSpeed(MAX);

	} else if (temp > 95)
	{
		setFanSpeed(90);
		checkLight=0;
	} else
	{
		checkLight=0;
		setFanSpeed(75);
	}
	temp = sensorData[OIL_PRESSURE];
	if (temp < 4)
	{

		shutDownEngine();
	}

	if(checkLight==1)
	{
		SwitchWarningLight(ON);
	}else{
		SwitchWarningLight(OFF);
	}
}

void oilTempCheck()
{


}

void shutDownEngine()
{
	
//TODO	actuate(GPIO,pin);
}
void setFanSpeed(uint8_t dutyCycle)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (dutyCycle * 10000) / 100;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}
void setCoolantPumpSpeed(uint8_t dutyCycle)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (dutyCycle * 10000) / 100;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}
