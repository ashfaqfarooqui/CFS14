#include "safety.h"

float olp_pressure = 0;
float olp_voltage = 0;
float clt_temp = 0;
float olt_temp = 0;
bol checkLight = FALSE;

TIM_OCInitTypeDef TIM_OCInitStructure;

void safetyCheck()
{
	checkLight = isEngineSafe();
	switchWarningLight(checkLight);
}

void coolingControl()
{
	
	float clt_temp = sensorData[WATER_TEMPRATURE];
	if (clt_temp <= 55)
	{
		setCoolantPumpSpeed(1500);
		setFanSpeed(1000);
	} else if (clt_temp > 55 && clt_temp < 85)
	{
		setCoolantPumpSpeed(1900);
		setFanSpeed(1500);
	} else if (clt_temp >= 85 && clt_temp < 105)
	{
		setCoolantPumpSpeed(2000);
		setFanSpeed(1900);
	} else
	{
		setFanSpeed(2000);
	}

}

bol isEngineSafe()
{
	
	//	uint8_t clt_data = sensorData[WATER_TEMPRATURE];
	//	float clt_voltage = convertData(clt_data);
	bol chkligt = FALSE;

	uint8_t olp_data = sensorData[OIL_PRESSURE];

	clt_temp = (sensorData[WATER_TEMPRATURE] - 32) * (5 / 9);
	if (clt_temp > 99)
	{
		if (sensorData[ENGINE_RPM] > 6000)
		{
			if (sensorData[OIL_PRESSURE] < 827)
			{
				chkligt = TRUE;
			} else
			{
				chkligt = FALSE;

			}
		} else
		{
			if (sensorData[OIL_PRESSURE] < 1)
			{
				chkligt = TRUE;
			} else
			{
				chkligt = FALSE;

			}
		}
	} else
	{
		if (sensorData[ENGINE_RPM] < 6000)
		{
			if (sensorData[OIL_PRESSURE] < 4)
			{
				chkligt = TRUE;
			} else
			{
				chkligt = FALSE;

			}
		} else
		{
			if (sensorData[OIL_PRESSURE] < 6)
			{
				chkligt = TRUE;
			} else
			{
				chkligt = FALSE;

			}
		}

	}

	return chkligt;

	//	if (clt_temp > 110)
	//	{
	//		checkLight = TRUE;
	//	} else if (clt_temp < 110)
	//	{
	//		checkLight = FALSE;
	//	}
	//
	//	if (olp_pressure < 3)
	//	{
	//		checkLight = TRUE;
	//	} else if (olp_pressure >= 3)
	//	{
	//		checkLight = FALSE;
	//	}
	//	if (checkLight == TRUE)
	//	{
	//		SwitchWarningLight(TRUE);
	//	} else
	//	{
	//		SwitchWarningLight(FALSE);
	//	}
}
void shutDownEngine()
{
	//actuate(GPIOA,KILL_ENGINE);
}
void setFanSpeed(uint16_t dutyCycle)
{
	if (dutyCycle > TIM3->ARR)
		dutyCycle = 0.98 * TIM3->ARR;

	TIM3->CCR1 = dutyCycle;
}
void setCoolantPumpSpeed(uint16_t dutyCycle)
{
	if (dutyCycle > TIM3->ARR)
		dutyCycle = 0.98 * TIM3->ARR;

	TIM3->CCR2 = dutyCycle;
}
