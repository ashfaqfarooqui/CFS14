/*
 * gearshfting.c
 *
 *  Created on: Mar 17, 2014
 *      Author: Zeyang Geng
 */
#include "gearshifting.h"

/********** GLOBAL DEFINATION **********/
int gear = 0;
int gearPosition = 0;
int gearPositionCurrent = 0;
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

const unsigned short shiftLevelsHigh[7] = {NEUTRALHIGH, GEAR1HIGH, GEAR2HIGH, GEAR3HIGH, GEAR4HIGH, GEAR5HIGH, GEAR6HIGH};
const unsigned short shiftLevelsLow[7] = {NEUTRALLOW, GEAR1LOW, GEAR2LOW, GEAR3LOW, GEAR4LOW, GEAR5LOW, GEAR6LOW};

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

	int shiftUpActiveTime = 0;
	int shiftDownActiveTime = 0;

	unsigned int sensorMonitor = 0;
	
	//int shiftUpDelay;	

	InactiveCutIgnition();
	ElClutch(rawDigitalState[EC_POS]);

	//LaunchControl();

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
		}
	}

	if (sensorMonitor > MONITOR_TIME)
	{
		ActiveClutch();
		problem = true;
		//put it into higher position
		sensorMonitor = 0;
	}

	if (!rawDigitalState[GEARUP_POS] && !rawDigitalState[GEARDOWN_POS])
	{
		shiftUpActive = false;
		shiftDownActive = false;
		goToNeutralActive = false;
		shiftDownHoldingTime = 0;
		shiftUpTime = 0;
		shiftDownTime = 0;
		return;
	}

	if (rawDigitalState[GEARUP_POS])
	{
		
		//GetGearPosition();

		if (gearIsInPosition)
		{
			if (gearPosition == 0)
			{ //now is in neutral
				flagNeutralToFirst = true;
				shiftDownActive = true;
				shiftUpActive = false;
				goToNeutralActive = false;
				neutralToGear = true;
			} else if (gearPosition > MAX_GEAR_POSITION - 1)
			{ //in gear 3 or over gear
				return;
			} else
			{
				flagFirstToSecond = true;
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

	while (rawDigitalState[GEARDOWN_POS] == 1)
	{
		shiftDownHoldingTime++;
		updateSwitches();
		shiftDownSwitch = true;
		
	}

	if (shiftDownSwitch == true)
	{
		shiftDownSwitch = false;
		/*
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
		{*/
		//want to shift down
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

		//}
	}

	if (shiftUpActive)
	{
		ShiftUp(gearPosition);
		delay(1000);
	}

	if (shiftDownActive)
	{
		ShiftDown(gearPosition);
		delay(1000);
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
	
	

	gearPositionSensorData = sensorData[GEAR_POSITION];

	
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

void ShiftUp(int gearPositionDeliver)

{
	
	int gearPositionMonitor = 0;

	flagShiftUp = true;
	ActiveCutIgnition();
	//delay(1000);
	//ActuateShiftUp(100);
	ActiveShiftUp();

	for (;; gearPositionMonitor++)
	{
//		portTickType a=xTaskGetTickCount();
		flagMonitoring = true;
		updateSwitches();
		saveRawADCData();
		gearPositionCurrent = GetGearPosition();
		if (gearIsInPosition && gearPositionCurrent == gearPositionDeliver + 1)
		{
			flagReachGear = true;
			break;
		}
		if(gearPositionMonitor > MONITOR_TIME){
			flagProblem = true;
			gearPositionMonitor = 0;
		  problem = true;
		  break;
		}
		//TODO
//		portTickType b=xTaskGetTickCount();
//		tim=b-a;
	}

	flagOutOfMonitoring = true;
	//delay(1000);
	//ActuateShiftUp(0);
	InactiveShiftUp();
	InactiveCutIgnition();
	
	flagReleaseShiftUp = true;

}

void ShiftDown(int gearPositionDeliver)
{
	//int gearPositionCurrent = 0;
	int gearPositionMonitor = 0;
	
	ActiveClutch();
	
	//delay(1000);
	
	//shiftDownTime = shiftDownTime + 50;

	ActiveShiftDown();

	for (;; gearPositionMonitor++)
	{
		updateSwitches();
		saveRawADCData();
		gearPositionCurrent = GetGearPosition();
		//shiftDownTime = shiftDownTime + 50;
		
		if (neutralToGear == false)
		{
			flagSecondToFirst = true;
			if (gearIsInPosition && gearPositionCurrent == gearPositionDeliver - 1)
			{
				flagFirstToNeutral = true;
				break;
			}
		}
		
		if(neutralToGear == true)
		{
			flagFirstToNeutral = true;
			if (gearIsInPosition && gearPositionCurrent == gearPositionDeliver + 1)
			{
				neutralToGear = false;
				break;
			}
		
		}

		if(gearPositionMonitor > MONITOR_TIME){
			gearPositionMonitor = 0;
		  problem = true;
		  break;
		}
	
	}
	InactiveClutch();
	InactiveShiftDown();

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

	ActiveClutch();
	
	for (;; gearPositionMonitor++)
	{

		if (timeLastingConter < timeLasting)
		{
			//simulate PWM output
			//xTaskGetTickCount();
			ActuateShiftUp(dutyCycle);
			delay(1000);
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

	ActuateShiftUp(0);
	InactiveClutch();
}

void ElClutch(int elClutch)
{

	if (elClutch == 1)
	{
		ActiveClutch();
		//flagElClutch = true;
	}
	if (elClutch == 0)
	{
		InactiveClutch();
		//flagElClutch = false;
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
	if (rawDigitalState[LC_POS] == 1 && launchControl == 0)
	{
		launchControl = 1;
		//TODO 1 actuate launch control to ECU
		//actuate();
		actuate(GPIOA, CLUTCH);
	}

	if (rawDigitalState[LC_POS] == 0 && launchControl == 1)
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

void ActiveClutch(){
	actuate(GPIOA,CLUTCH);
	flagElClutch = true;
}

void InactiveClutch(){
	release(GPIOA,CLUTCH);
	flagElClutch = false;
}

void ActiveShiftUp(){
	actuate(GPIOC,SHIFT_UP);
}

void InactiveShiftUp(){
	release(GPIOC,SHIFT_UP);
}
void ActiveShiftDown(){
	actuate(GPIOC,SHIFT_DOWN);
}

void InactiveShiftDown(){
	release(GPIOC,SHIFT_DOWN);
}

void ActiveCutIgnition(){
	release(GPIOE,CUT_IGNITION);
}

void InactiveCutIgnition(){
	actuate(GPIOE,CUT_IGNITION);
}
