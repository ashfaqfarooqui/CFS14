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
	
	float clt_temp = (sensorData[WATER_TEMPRATURE] - 32) * (5 / 9);
	if (clt_temp <= 55)
	{
		setCoolantPumpSpeed(7500);
		setFanSpeed(5000);
	} else if (clt_temp > 55 && clt_temp < 85)
	{
		setCoolantPumpSpeed(9000);
		setFanSpeed(7500);
	} else if (clt_temp >= 85 && clt_temp < 105)
	{
		setCoolantPumpSpeed(10000);
		setFanSpeed(9000);
	} else
	{
		setFanSpeed(10000);
	}

}

bol isEngineSafe()
{
	
	bol chkligt = FALSE;

		clt_temp = ((float) (((sensorData[WATER_TEMPRATURE]/10) - 32) * 5 )/ 9);
//	if (clt_temp > 99)
//	{
//		if (sensorData[ENGINE_RPM] > 6000)
//		{
//			if (sensorData[OIL_PRESSURE] < 827)
//			{
//				chkligt = TRUE;
//			} else
//			{
//				chkligt = FALSE;
//
//			}
//		} else
//		{
//			if (sensorData[OIL_PRESSURE] < 1)
//			{
//				chkligt = TRUE;
//			} else
//			{
//				chkligt = FALSE;
//
//			}
//		}
//	} else
//	{
//		if (sensorData[ENGINE_RPM] < 6000)
//		{
//			if (sensorData[OIL_PRESSURE] < 4)
//			{
//				chkligt = TRUE;
//			} else
//			{
//				chkligt = FALSE;
//
//			}
//		} else
//		{
//			if (sensorData[OIL_PRESSURE] < 6)
//			{
//				chkligt = TRUE;
//			} else
//			{
//				chkligt = FALSE;
//
//			}
//		}
//
//	}

	if (clt_temp > 105)
	{
		chkligt = TRUE;
	}
	
	if (sensorData[OIL_PRESSURE] < 1400&& sensorData[ENGINE_RPM]>5500)
	{
		chkligt = TRUE;
	}
	return chkligt;

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
