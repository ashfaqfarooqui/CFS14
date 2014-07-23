#include "gearshifting.h"

/********** GLOBAL DEFINATION **********/
uint8_t gear = 0;
uint8_t gearPosition = 0;
uint8_t gearPositionCurrent = 0;
bool gearIsInPosition = false;
bool problem = false;
bool neutralToGear = false;
int gearPositionSensorData = 0;
unsigned int shiftDownHoldingTime = 0;

bool shiftUpActive = false;
bool shiftDownActive = false;
bol goToNeutral = FALSE;
bol ToNeutral = FALSE;
bol autoShiftSwitch = FALSE;
bol autoShiftBlocked = FALSE;

//bool shiftDownSwitch = false;
uint16_t blockingTime = 0;
bool autoShifting = false;
uint16_t dutyCycle = 20;
int timeLastingConter = 0;
int timeLasting = 0;

int shiftUpTime = 0;
int shiftDownTime = 0;

const unsigned short shiftLevelsHigh[7] = {
NEUTRALHIGH, GEAR1HIGH, GEAR2HIGH, GEAR3HIGH, GEAR4HIGH, GEAR5HIGH, GEAR6HIGH
};
const unsigned short shiftLevelsLow[7] = {
NEUTRALLOW, GEAR1LOW, GEAR2LOW, GEAR3LOW, GEAR4LOW, GEAR5LOW, GEAR6LOW
};

bool flagShiftUp = false;
bool flagMonitoring = false;
bool flagReachGear = false;
bool flagOutOfMonitoring = false;
bool flagReleaseShiftUp = false;
bool flagNeutralToFirst = false;
bool flagFirstToSecond = false;
bool flagElClutch = false;
bool flagSecondToFirst = false;
bool flagFirstToNeutral = false;
bool flagProblem = false;
/***************************************/

void gearShiftManager(void)
{

	unsigned int sensorMonitor = 0;
	
	InactiveCutIgnition();
	ElClutch((ElectricClutchActuated == TRUE)/*
	 || (LaunchControlActivated == TRUE)*/);

	GetGearPosition();

	if (!gearIsInPosition)
	{
		sensorMonitor++;
	} else
	{
		if (problem)
		{
			//InactiveClutch();
			InactiveShiftUp();
			InactiveShiftDown();
			sensorMonitor = 0;
			problem = false;
			shiftUpSwitch = FALSE;
			shiftDownSwitch = FALSE;
			goToNeutral = FALSE;
		}
	}

	if (sensorMonitor > MONITOR_TIME)
	{
		ActiveClutch();
		problem = true;
		//put it into higher position
		sensorMonitor = 0;
	}

	if (shiftDownSwitch == FALSE && shiftUpSwitch == FALSE)
	{
		shiftUpActive = false;
		shiftDownActive = false;

		shiftDownHoldingTime = 0;
		shiftUpTime = 0;
		shiftDownTime = 0;
		return;
	}

	if (shiftUpSwitch == TRUE && shiftDownSwitch == FALSE)
	{

		//GetGearPosition();
		shiftUpSwitch = FALSE;

		if (gearIsInPosition)
		{
			if (gearPosition == 0)
			{ //now is in neutral
//				flagNeutralToFirst = true;
				shiftDownActive = true;
				shiftUpActive = false;

				neutralToGear = true;
			} else if (gearPosition > MAX_GEAR_POSITION - 1)
			{ //in gear 3 or over gear
				return;
			} else
			{
//				flagFirstToSecond = true;
				shiftUpActive = true;
				shiftDownActive = false;
				
			}
		} else
		{
			problem = true;
			return;
		}

	}

	if (shiftDownSwitch == TRUE && shiftUpSwitch == FALSE)
	{
		shiftDownSwitch = FALSE;
		if (gearIsInPosition)
		{
			if (gearPosition == 0)
			{		//now is in neutral, cannot shift down
				return;
			} else if (gearPosition == 1)
			{
				//now is in gear1, cannot shift down
				if (ElectricClutchActuated == TRUE)
				{
					neutralSwitch = TRUE;
				}
				return;
			} else
			{
				if ((sensorData[ENGINE_RPM] < SHIFTDOWN2RPM_PROTECTION
						&& gearPosition == 2)
						|| (sensorData[ENGINE_RPM] < SHIFTDOWN3RPM_PROTECTION
								&& gearPosition == 3))
				{
					//TODO
					shiftDownActive = true;
					shiftUpActive = false;
				}
			}
		} else
		{
			problem = true;
			return;
		}

	}

	if (shiftDownSwitch == TRUE && shiftUpSwitch == TRUE)
	{
		shiftDownSwitch = FALSE;
		shiftUpSwitch = FALSE;
	}

	actuateShift();
	delay(20);
	sendGear();
	delay(20);
}

void actuateShift()
{
	IWDG_ReloadCounter();
	if (shiftUpActive==true)
	{
		shiftUpSwitch = FALSE;
		shiftDownSwitch = FALSE;
		ShiftUp(gearPosition);
		stopTimer();
		sendGearTime(SHIFT_DIRECTION_UP);
		shiftUpActive = false;
		delay(25);
		sendGearTime(SHIFT_DIRECTION_UP);
	}

	if (shiftDownActive==true)
	{
		shiftUpSwitch = FALSE;
		shiftDownSwitch = FALSE;
		ShiftDown(gearPosition);
		stopTimer();
		sendGearTime(SHIFT_DIRECTION_DOWN);
		shiftDownActive = false;
		delay(25);
		sendGearTime(SHIFT_DIRECTION_DOWN);
	}

}
int GetGearPosition()
{

	gearPositionSensorData = sensorData[GEAR_POSITION];
	gear = 0;
	gearIsInPosition = false;
	for (gear = 0; gear < 6; gear++)
	{
		if (gearPositionSensorData > shiftLevelsLow[gear]
				&& gearPositionSensorData < shiftLevelsHigh[gear])
		{
			gearPosition = gear;
			gearIsInPosition = true;
			break;
		}

		if (gearPositionSensorData >= shiftLevelsHigh[gear]
				&& gearPositionSensorData <= shiftLevelsLow[gear + 1])
		{
			gearPosition = 8;
			gearIsInPosition = false;
			break;
		}
	}

	return gearPosition;

}

void ShiftUp(int gearPositionDeliver)

{

	uint32_t gearPositionMonitor = 0;

//	flagShiftUp = true;
	ActiveCutIgnition();
	ActuateShiftUp(PERIOD_GEAR);
//	ActiveShiftUp();

	for (;; gearPositionMonitor++)
	{
//		flagMonitoring = true;

		saveRawADCData();
		gearPositionCurrent = GetGearPosition();
		if (gearIsInPosition && gearPositionCurrent == gearPositionDeliver + 1)
		{
//			flagReachGear = true;
			break;
		} else if (gearPositionMonitor > MONITOR_TIME)
		{
//			flagProblem = true;
			gearPositionMonitor = 0;

			problem = true;
			break;
		}
	}

	flagOutOfMonitoring = true;
	ActuateShiftUp(0);
//	InactiveShiftUp();
	InactiveCutIgnition();

//	flagReleaseShiftUp = true;

}

void ShiftDown(int gearPositionDeliver)
{
	uint32_t gearPositionMonitor = 0;

	ActiveClutch();

	delay(150);
	ActuateShiftDown(PERIOD_GEAR);
//	ActiveShiftDown();

	for (;; gearPositionMonitor++)
	{
		saveRawADCData();
		gearPositionCurrent = GetGearPosition();

		if (neutralToGear == false)
		{
//			flagSecondToFirst = true;
			if (gearIsInPosition
					&& gearPositionCurrent == gearPositionDeliver - 1)
			{
//				flagFirstToNeutral = true;
				break;
			}
		}

		else if (neutralToGear == true)
		{
//			flagFirstToNeutral = true;
			if (gearIsInPosition
					&& gearPositionCurrent == gearPositionDeliver + 1)
			{
//				neutralToGear = false;
				break;
			}

		}

		if (gearPositionMonitor > MONITOR_TIME)
		{
			gearPositionMonitor = 0;
			problem = true;
			break;
		}

	}
	InactiveClutch();
//	InactiveShiftDown();
	ActuateShiftDown(0);
	return;
}

void neutralMgr(void)
{

	GetGearPosition();
	if (gearPosition == 1)
	{
		ToNeutral = TRUE;
	}
	if (gearPosition != 0 && gearPosition >!1)
	{
		ActuateShiftUp(dutyCycle);

		dutyCycle = (dutyCycle + 2) % 85;
	}
	if ((gearIsInPosition && gearPosition == 0) || gearPosition > 1)
	{
		ActuateShiftUp(0);
		dutyCycle = 0;
		neutralSwitch = FALSE;
		ToNeutral = FALSE;
	}

}

void ElClutch(bol elClutch)
{

	if (elClutch == TRUE)
	{
		actuate(GPIOA, CLUTCH);
		//flagElClutch = true;
	}
	if (elClutch == FALSE)
	{
		release(GPIOA, CLUTCH);
		//flagElClutch = false;
	}
}

void autoShiftManager(void)
{

	uint16_t rpm = sensorData[ENGINE_RPM];
	GetGearPosition();
	if ((autoShiftSwitch == TRUE && autoShiftBlocked == FALSE)
			&& (gearPosition == 1 && rpm > 13500)
			|| (gearPosition == 2 && rpm > 13000))
	{
		shiftUpActive = true;
		startTimer();
		autoShiftBlocked = TRUE;
		actuateShift();
	}
	if (autoShiftBlocked == TRUE)
	{
		blockingTime++;
		if (blockingTime > 50000)
		{
			autoShiftBlocked = FALSE;
			blockingTime = 0;
		}

	}
}

void ActiveClutch()
{
	actuate(GPIOA, CLUTCH);
	flagElClutch = true;
}

void InactiveClutch()
{
	release(GPIOA, CLUTCH);
	flagElClutch = false;
}

void ActiveShiftUp()
{
	actuate(GPIOC, SHIFT_UP);
}

void InactiveShiftUp()
{
	release(GPIOC, SHIFT_UP);
}
void ActiveShiftDown()
{
	actuate(GPIOC, SHIFT_DOWN);
}

void InactiveShiftDown()
{
	release(GPIOC, SHIFT_DOWN);
}

void ActiveCutIgnition()
{
	release(GPIOE, CUT_IGNITION);
}

void InactiveCutIgnition()
{
	actuate(GPIOE, CUT_IGNITION);
}
