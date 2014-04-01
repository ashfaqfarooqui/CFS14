/*
 * gearshfting.c
 *
 *  Created on: Mar 17, 2014
 *      Author: Zeyang Geng
 */

#include "gearshifting.h"

//How to go neutrual
// may be press both for long time


/********** GLOBAL DEFINATION **********/
unsigned int gearPosition = 0;
bool gearIsInPosition = false;
bool problem = false;
bool autoShiftingBlock = true;

int shiftUpTime = 0;
int shiftDownTime = 0;

const unsigned short shiftLevelsHigh[7] =
	{NEUTRALHIGH, GEAR1HIGH, GEAR2HIGH, GEAR3HIGH, GEAR4HIGH, GEAR5HIGH, GEAR6HIGH};
const unsigned short shiftLevelsLow[7] =
	{NEUTRALLOW, GEAR1LOW, GEAR2LOW, GEAR3LOW, GEAR4LOW, GEAR5LOW, GEAR6LOW};

/***************************************/
void init_actuators()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =CLUTCH|SHIFT_UP|SHIFT_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GEAR_SHIFT_PORT, &GPIO_InitStructure);
}

void actuate(GPIO_TypeDef* port,uint16_t system)
{
	GPIO_SetBits(port,system);
}


void release(GPIO_TypeDef* port,uint16_t system)
{
	GPIO_ResetBits(port,system);
}

//how to call actuator and read ADC value
//
//actuate(GEAR_SHIFT_PORT,SHIFT_UP)
//
//gearPositionSensor = rawAnalogState[1]
//front wheel speed left = rawAnalogState[2]
//front wheel speed right = rawAnalogState[3]
//
//Digital input
//gearup digIn[0] = 1
//geardown digIn[1] = 1
//Elclutch digIn[2] = 1
//Auto digIn[3] = 1


/****************************************/


void gearShiftManager(void) {
	void init_actuators();
	static unsigned int shiftDownHoldingTime = 0;

	int shiftUpSwitch, shiftDownSwitch;
	bool shiftUpActive, shiftDownActive, goToNeutralActive;

	unsigned int sensorMonitor = 0;

	ElClutch(digIn[2]);

	gearIsInPosition = GearIsInPosition();
	gearPosition = GetGearPosition();

	if (!gearIsInPosition){
			sensorMonitor++;
	}else{
		if(problem){
			realse(GEAR_SHIFT_PORT,CLUTCH);
			sensorMonitor = 0;
		}
	}

	if (sensorMonitor > MONITOR_TIME){
		actuate(GEAR_SHIFT_PORT,CLUTCH);
		problem = true;
		//put it into higher position
	    sensorMonitor = 0;
	}

	if(!digIn[0] && !digIn[1]){
		shiftUpActive = false;
		shiftDownActive = false;
		goToNeutralActive = false;
		shiftDownSwitchHoldingTime = 0;
		shiftUpUsedTime = 0;
		shiftDownUsedTime = 0;
		goToNeutralUsedTime = 0;

		return;
	}

	if(digIn[0]){
		gearIsInPosition = GearIsInPosition();
		gearPosition = GetGearPosition();

		if(gearIsInPosition){
			if(gearPosition = 0){//now is in neutral
				shiftDownActive = true;
				shiftUpActive = false;
				goToNeutral = false;
			}else if(gearPosition > MAX_GEAR_POSITION - 1){//in gear 3 or over gear
				return;
			}else{
				shiftUpActive = true;
				shiftDownActive = false;
				goToNeutralActive = false;
			}
		}else{
			problem = true;
			return;
		}

		//shiftUpUsedTime = shiftUpUsedTime + HoldingTime;
		//TODO
	}

	while(digIn[1]){
		shiftDownSwitchHoldingTime++;
	}

	if(shiftDownSwitchHoldingTime > SWITCHHOLDINGTIME){
		//holding shift down for long time mean go to neutral
		gearIsInPosition = GearIsInPosition();
		gearPosition = GetGearPosition();

		if(gearIsInPosition){
			if(gearPosition = 1){//in gear 1 and go to neutral
				shiftDownSwitchHoldingTime = 0;
				goToNeutralActive = true;
				shiftUpActive = false;
				shiftDownActive = false;
			}
		}else{
			problem = true;
			return;
		}

	}else{//want to shift down
		shiftDownSwitchHoldingTime = 0;
		gearIsInPosition = GearIsInPosition();
		gearPosition = GetGearPosition();
		if(gearIsInPosition){
			if(gearPosition == 0){//now is in neutral, cannot shift down
				return;
			}else if(gearPosition == 1){//now is in gear1, cannot shift down
				return;
			}else{
				//TODO
				//PT
				//Shift down speed protect
				shiftDownActive = true;
				shiftUpActive = false;
				goToNeutralActive = false;
			}
		}else{
			problem = true;
			return;
		}

	}


	if(shiftUpActive){
		actuate(GEAR_SHIFT_PORT,SHIFT_UP);
		for(int i = 0; i < 50; i++){

		}

		release(GEAR_SHIFT_PORT,SHIFT_UP);
	}

	if(shiftDownActive){
		ShiftDown(gearPosition);
	}

	if(goToNeutralActive){
		GoToNeutral();
	}
	if(digIn[3] == 1){
		AutoShifting();
	}
}

bool GearIsInPosition(void){
	uint16_t gearPositionSensorData = rawAnalogState[1];

	for(int i = 0 ; i < 7 ; i++){
		if (gearPositionSensorData > shiftLevelsLow[i] && gearPositionSensorData < shiftLevelsHigh[i]){
			return true;
		}
	}

	return false;
}

Int GetGearPosition(){

	uint16_t gearPositionSensorData = rawAnalogState[1];

	int gearPosition = 0;
	for(int i = 0 ; i < 7 ; i++){
		if (gearPositionSensorData > shiftLevelsLow[i] && gearPositionSensorData < shiftLevelsHigh[i]){
			gearPosition = i;
			gearIsInPosition = true;
			break;
			}

		if (gearPositionSensorData > shiftLevelsHigh[i] && gearPositionSensorData < shiftLevelsLow[i+1]){
			gearPosition = i;
			gearIsInPosition = false;
			break;
			}
	}

	return gearPosition;

}

void ShiftDown(int gearPosition){

	actuate(GEAR_SHIFT_PORT,CLUTCH);

	for(int i = 0 ; i< 50; i++){
		;
	}
	shiftDownTime = shiftDownTime + 50;

	actuate(GEAR_SHIFT_PORT,SHIFT_DOWN);

	int gearPositionCurrent = 0;

	for(int gearPositionMonitor = 0 ; ; gearPositionMonitor++){
		gearPositionCurrent = GetGearPosition();
		for(int i = 0 ; i< 50; i++){
				;
			}
		shiftDownTime = shiftDownTime + 50;
		if(gearPositionCurrent = gearPosition - 1){
			break;
		}
		if(gearPositionMonitor > MONITOR_TIME){
			gearPositionMonitor = 0;

		}
	}

	release(GEAR_SHIFT_PORT,CLUTCH);
	release(GEAR_SHIFT_PORT,SHIFT_DOWN);

	return;
}

void GoToNeutral(void){
	actuate(GEAR_SHIFT_PORT,NEUTRAL);

	//how can we know that the neutral cylinder is in position?
	//Or wait for some time to let it in position?
	//what if this one failed
	//TODO Toni

	//delay

	actuate(GEAR_SHIFT_PORT,SHIFT_UP);
	//Use the function in ECU
	//TODO
	//Toni

	for(int i = 0; i < 50; i++){

	}

	release(GEAR_SHIFT_PORT,SHIFT_UP);
	release(GEAR_SHIFT_PORT,NEUTRAL);
}


void ElClutch(int elClutch){

	if(elClutch == 1){
		actuate(GEAR_SHIFT_PORT,CLUTCH);
	}
	if(elClutch == 0){
		release(GEAR_SHIFT_PORT,CLUTCH);
	}
}

void AutoShifting(void){
	//TODO
	//ashfaq
	//read engine speed through CAN or....????

	//int engineSpeed =
	//int vehicalSpeed = (rawAnalogState[2] + rawAnalogState[3])/2;




}
