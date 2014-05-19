#include "safety.h"

void safetyCheck()
{
	uint8_t clt_data = sensorData[WATER_TEMPRATURE];
	float clt_voltage = convertData(clt_data);
	float clt_temp = -29.2793*clt_voltage + 168.8014;

	uint8_t olt_data = sensorData[OIL_TEMPRATURE];
	float olt_voltage = convertData(olt_data);
	float olt_temp = -38.1625*olt_voltage + 199.9477;

	uint8_t olp_data = sensorData[OIL_PRESSURE];
	float olp_voltage = convertData(olp_data);
	float olp_pressure = 1.7489*olp_voltage - 0.3722;

	uint8_t checkLight=1;

	if (clt_temp > 120)
	{
		shutDownEngine();

	} else if (clt_temp > 110)
	{
		setFanSpeed(MAX);

	} else if (clt_temp > 95)
	{
		setFanSpeed(90);
		checkLight=0;
	} else
	{
		checkLight=0;
		setFanSpeed(75);
	}


	if(olt_temp>120){
		checkLight=1;
	}

	if (olp_pressure < 4)
	{
		checkLight=1;
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
