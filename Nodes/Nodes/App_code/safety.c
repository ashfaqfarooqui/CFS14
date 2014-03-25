#include "safety.h"

//TODO put tempratures into variables!
uint8_t getTemprature(uint8_t param)
{
	if (param == OIL)
	{
		//send can rtr to get oil temp from ECU
	}
	if (param == WATER)
	{
		//send can rtr to get water temp from ECU
	}

}

void waterTempCheck()
{
	uint8_t temp = getTemprature(OIL);
	if (temp > 120)
	{
		//TODO: shutdown engine
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

	uint8_t temp = getTemprature(WATER);
	if (temp > 120)
	{
		//TODO: shutdown engine
	}

}

void setFanSpeed(uint8_t dutyCycle)
{
	uint16_t pwmSpeed=(dutyCycle*665)/100;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pwmSpeed;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	//TODO write pwm code to control the driver motor.
}
