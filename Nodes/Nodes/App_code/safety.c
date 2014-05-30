#include "safety.h"
float olp_pressure;
float olp_voltage;
void safetyCheck()
{
	uint8_t checkLight = 0;
	uint8_t clt_data = sensorData[WATER_TEMPRATURE];
	float clt_voltage = convertData(clt_data);
	float clt_temp = -29.2793 * clt_voltage + 168.8014;

	uint8_t olt_data = sensorData[OIL_TEMPRATURE];
	float olt_voltage = convertData(olt_data);
	float olt_temp = -38.1625 * olt_voltage + 199.9477;

	uint8_t olp_data = sensorData[OIL_PRESSURE];
	 olp_voltage = (float)(((1.1407*(float)(sensorData[OIL_PRESSURE]))/1000)-0.0312);
	olp_pressure = 2.5 * olp_voltage -2;
	

	if (clt_temp > 110)
	{
		checkLight = TRUE;
	} else if (clt_temp < 110)
	{
		checkLight = FALSE;
	}

	if (olt_temp > 110)
	{
		checkLight = TRUE;
	} else if (clt_temp < 110)
	{
		checkLight = FALSE;
	}
	if (olp_pressure < 4)
	{
		checkLight = TRUE;
	}
	if (checkLight == TRUE)
	{
		SwitchWarningLight(ON);
	} else
	{
		SwitchWarningLight(OFF);
	}
}
	void oilTempCheck()
	{
	}

	void coolingControl()
	{
		float clt_voltage = convertData(sensorData[WATER_TEMPRATURE]);
		float clt_temp = -29.2793 * clt_voltage + 168.8014; //TODO: convert to exact value
		if (clt_temp <= 55)
		{
			setCoolantPumpSpeed(70);
			setFanSpeed(30);
		} else if (clt_temp > 55 && clt_temp < 85)
		{
			setCoolantPumpSpeed(90);
			setFanSpeed(70);
		} else if (clt_temp >= 85 && clt_temp <105)
		{
			setCoolantPumpSpeed(100);
			setFanSpeed(90);
		} else
		{
			setFanSpeed(100);
		}

	}

	void shutDownEngine()
	{
		//actuate(GPIOA,KILL_ENGINE);
	}
	void setFanSpeed(uint8_t dutyCycle)
	{
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
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
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = (dutyCycle * 10000) / 100;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC2Init(TIM3, &TIM_OCInitStructure);

		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE);
	}
