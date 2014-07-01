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
	uint16_t speed = PERIOD_FAN;
	clt_temp = ((((float) sensorData[WATER_TEMPRATURE] / 10 - 32) * 5) / 9);
	if (clt_temp >= 96)
	{
		//setCoolantPumpSpeed(100);
		speed = 0;
//	} else if (clt_temp < 100 && clt_temp > 105)
//	{
//		//	setCoolantPumpSpeed(50);
//		speed = 10;
	} else if (clt_temp > 90 && clt_temp < 95)
	{
		//	setCoolantPumpSpeed(25);
		speed = (PERIOD_FAN*4)/10;
	} else if (clt_temp <90  && clt_temp > 86)
	{
		speed = (PERIOD_FAN*8)/10;
	}
	if (clt_temp < 70)
	{
		speed = PERIOD_FAN;
	}
	setFanSpeed(speed);

	///P controler
//	setTemp=90;
//	error=clt_temp-setTemp;
//	if(error>0)
//	{
//
//	}

}

bol isEngineSafe()
{
	
	bol chkligt = FALSE;

	clt_temp = ((float) (((sensorData[WATER_TEMPRATURE] / 10) - 32) * 5) / 9);
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
	
	if (sensorData[OIL_PRESSURE] < 1400 && sensorData[ENGINE_RPM] > 5500)
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
	

	TIM3->CCR1 = dutyCycle;
}
void setCoolantPumpSpeed(uint16_t dutyCycle)
{
	if (dutyCycle > TIM3->ARR)
		dutyCycle = 0.98 * TIM3->ARR;

	TIM3->CCR2 = dutyCycle;
}
