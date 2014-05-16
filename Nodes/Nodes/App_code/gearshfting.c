/*
 * gearshfting.c
 *
 *  Created on: Mar 17, 2014
 *      Author: Zeyang Geng
 */
#include "gearshifting.h"

/********** GLOBAL DEFINATION **********/
int gearPosition = 0;
bool gearIsInPosition = false;
bool problem = false;
bool neutralToGear = false;
bool autoShiftingBlock = true;
int gearPositionSensorData = 0;
unsigned int shiftDownHoldingTime = 0;

bool shiftUpActive = false;
bool shiftDownActive = false;
bool goToNeutralActive = false;

bool shiftDownSwitch = false;

int launchControl = 0;
bool autoShifting = false;
uint8_t dutyCycle = 0;
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

/***************************************/

void gearShiftManager(void)
{
	
	int shiftUpActiveTime = 0;
	int shiftDownActiveTime = 0;

	unsigned int sensorMonitor = 0;
	
	//int shiftUpDelay;	

	ElClutch(rawDigitalState[7]);

	//LaunchControl();

	GetGearPosition();
	
	if (!gearIsInPosition)
	{
		sensorMonitor++;
	} else
	{
		if (problem)
		{
			actuate(GPIOC, CLUTCH);
			sensorMonitor = 0;
			problem = false;
		}
	}

	if (sensorMonitor > MONITOR_TIME)
	{
		actuate(GPIOC, CLUTCH);
		problem = true;
		//put it into higher position
		sensorMonitor = 0;
	}

	if (!rawDigitalState[0] && !rawDigitalState[1])
	{
		shiftUpActive = false;
		shiftDownActive = false;
		goToNeutralActive = false;
		shiftDownHoldingTime = 0;
		shiftUpTime = 0;
		shiftDownTime = 0;
		return;
	}

	if (rawDigitalState[0])
	{
		
		//GetGearPosition();

		if (gearIsInPosition)
		{
			if (gearPosition == 0)
			{ //now is in neutral
				shiftDownActive = true;
				shiftUpActive = false;
				goToNeutralActive = false;
				neutralToGear = true;
			} else if (gearPosition > MAX_GEAR_POSITION - 1)
			{ //in gear 3 or over gear
				return;
			} else
			{
				shiftUpActive = true;
				shiftDownActive = false;
				goToNeutralActive = false;
			}
		} else
		{
			problem = true;
			return;
		}

		//shiftUpUsedTime = shiftUpUsedTime + HoldingTime;
		//TODO
	}

	while (rawDigitalState[1] == 1)
	{
		shiftDownHoldingTime++;
		updateSwitches();
		shiftDownSwitch = true;
		
	}

	if (shiftDownSwitch == true)
	{
		shiftDownSwitch = false;
		if (shiftDownHoldingTime > SWITCHHOLDINGTIME)
		{
			//holding shift down for long time mean go to neutral

			//GetGearPosition();

			if (gearIsInPosition)
			{
				if (gearPosition == 1)
				{		//in gear 1 and go to neutral
					shiftDownHoldingTime = 0;
					goToNeutralActive = true;
					shiftUpActive = false;
					shiftDownActive = false;
				}
			} else
			{
				problem = true;
				return;
				//Gear is not in position or driver want to go to neutral in other gear, maybe something wrong
			}

		} else
		{		//want to shift down
			shiftDownHoldingTime = 0;
			//GetGearPosition();
			if (gearIsInPosition)
			{
				if (gearPosition == 0)
				{		//now is in neutral, cannot shift down
					return;
				} else if (gearPosition == 1)
				{		//now is in gear1, cannot shift down
					return;
				} else
				{
					//if((rpm < SHIFTDOWN2RPM_PROTECTION && gearPosition == 2) ||
					//    (rpm < SHIFTDOWN3RPM_PROTECTION && gearPosition == 3)){
					//TODO
					shiftDownActive = true;
					shiftUpActive = false;
					goToNeutralActive = false;
					//}
				}
			} else
			{
				problem = true;
				return;
			}

		}
	}

	if (shiftUpActive)
	{
		ShiftUp(gearPosition);
	}

	if (shiftDownActive)
	{
		ShiftDown(gearPosition);
	}

	if (goToNeutralActive)
	{
		GoToNeutral();
	}
	/*
	 if(digIn[3] == 1){
	 AutoShifting();
	 }
	 */

	//GoToNeutral();
}

int GetGearPosition()
{
	
	int gear = 0;
	
	gearPositionSensorData = (int)(convertData(sensorData[GEAR_POSITION])*1000);

	
	for (gear = 0; gear < 6; gear++)
	{
		if (gearPositionSensorData > shiftLevelsLow[gear]
				&& gearPositionSensorData < shiftLevelsHigh[gear])
		{
			gearPosition = gear;
			gearIsInPosition = true;
			break;
		}

		if (gearPositionSensorData > shiftLevelsHigh[gear]
				&& gearPositionSensorData < shiftLevelsLow[gear + 1])
		{
			gearPosition = gear;
			gearIsInPosition = false;
			break;
		}
	}
	gear = 0;

	return gearPosition;

}

void ShiftUp(int gearPosition)

{
	int gearPositionCurrent = 0;
	int gearPositionMonitor = 0;

	actuate(GPIOC, CUT_IGNITION);
	//delay(1000);
	actuateShiftUpSolonoid(100);

	for (;; gearPositionMonitor++)
	{
		updateSwitches();
		saveRawADCData();
		gearPositionCurrent = GetGearPosition();
		if (gearIsInPosition && gearPositionCurrent == gearPosition + 1)
		{
			break;
		}
		//if(gearPositionMonitor > MONITOR_TIME){
		//	gearPositionMonitor = 0;
		//  problem = true;
		//  break;
		//}
		//TODO
	}

	//delay(1000);
	actuateShiftUpSolonoid(0);
	release(GPIOC, CUT_IGNITION);
	
}

void ShiftDown(int gearPosition)
{
	int gearPositionCurrent = 0;
	int gearPositionMonitor = 0;
	
	actuate(GPIOC, CLUTCH);
	
	//delay(1000);
	
	//shiftDownTime = shiftDownTime + 50;

	actuate(GPIOA, SHIFT_DOWN);

	for (;; gearPositionMonitor++)
	{
		updateSwitches();
		saveRawADCData();
		gearPositionCurrent = GetGearPosition();
		shiftDownTime = shiftDownTime + 50;
		if (neutralToGear == false)
		{
			if (gearIsInPosition && gearPositionCurrent == gearPosition - 1)
			{
				break;
			}
		} else
		{
			if (gearIsInPosition && gearPositionCurrent == gearPosition + 1)
			{
				neutralToGear = false;
				break;
			}
		}
		//if(gearPositionMonitor > MONITOR_TIME){
		//	gearPositionMonitor = 0;
		//
		//}
	}

	release(GPIOA, CLUTCH);
	release(GPIOA, SHIFT_DOWN);

	return;
}

void GoToNeutral(void)
{
	int gearPositionCurrent = 0;
	int gearPositionMonitor = 0;
	
	uint8_t dutyCycleStep = 10;

	dutyCycle = 0;
	timeLastingConter = 0;
	timeLasting = 10;

	actuate(GPIOA, CLUTCH);
	
	for (;; gearPositionMonitor++)
	{

		if (timeLastingConter < timeLasting)
		{
			//simulate PWM output
			actuateShiftUpSolonoid(dutyCycle);
			timeLastingConter = (timeLastingConter + 1) % 10;
		}

		if (timeLastingConter == 9)
		{
			dutyCycle = dutyCycle + dutyCycleStep;
		}

		saveRawADCData();
		gearPositionCurrent = GetGearPosition();

		if (neutralToGear == false || dutyCycle == 100)
		{
			if (gearPositionCurrent != 1)
			{
				break;
			}
		}
		//if(gearPositionMonitor > MONITOR_TIME){
		//	gearPositionMonitor = 0;
		//
		//}

	}

	actuateShiftUpSolonoid(0);
	release(GPIOA, CLUTCH);
}

void ElClutch(int elClutch)
{

	if (elClutch == 1)
	{
		actuate(GPIOC, GPIO_Pin_15);
	}
	if (elClutch == 0)
	{
		release(GPIOC, GPIO_Pin_15);
	}
}

void AutoShifting(void)
{
	//TODO

	//int engineSpeed =
	//int vehicalSpeed = (rawAnalogState[2] + rawAnalogState[3])/2;

}

void LaunchControl(void)
{
	if (rawDigitalState[1] == 1 && launchControl == 0)
	{
		launchControl = 1;
		//TODO 1 actuate launch control to ECU
		//actuate();
		actuate(GPIOC, CLUTCH);
	}

	if (rawDigitalState[1] == 0 && launchControl == 1)
	{
		//just after release button
		//TODO 2
		//lauchControl == 2;//car going to move
		release(GPIOC, CLUTCH);
		//TODO 2
		//clutchDelay = LAUCH_CONTROL_CLUTCH_ON_DELAY;
		delay(150);
		//TODO 1
		//release();
		launchControl = 0;
	}

	//TODO 2
	/*
	 if(rawDigitalState[1] == 0 && launchControl == 2){
	 if(clutchDelay){
	 clutchDelay--;
	 }else{
	 launchControl = 0;
	 //TODO 1
	 //release();
	 }
	 }
	 */

}
