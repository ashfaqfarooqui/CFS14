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
int gearPosition = 0;
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin =SHIFT_UP|SHIFT_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GEAR_SHIFT_PORT, &GPIO_InitStructure);
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
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

	static unsigned int shiftDownHoldingTime = 0;

	//int shiftUpSwitch, shiftDownSwitch;
	bool shiftUpActive = false;
	bool shiftDownActive = false;
	bool goToNeutralActive = false;

	unsigned int sensorMonitor = 0;
	
	int shiftUpDelay;	

	ElClutch(rawDigitalState[0]);

	GetGearPosition();

	if (!gearIsInPosition){
			sensorMonitor++;
	}else{
		if(problem){
			release(GEAR_SHIFT_PORT,CLUTCH);
			sensorMonitor = 0;
		}
	}

	if (sensorMonitor > MONITOR_TIME){
		actuate(GEAR_SHIFT_PORT,CLUTCH);
		problem = true;
		//put it into higher position
	    sensorMonitor = 0;
	}

	if(!rawDigitalState[0] && !rawDigitalState[1]){
		shiftUpActive = false;
		shiftDownActive = false;
		goToNeutralActive = false;
		shiftDownHoldingTime = 0;
		shiftUpTime = 0;
		shiftDownTime = 0;
		return;
	}

	if(rawDigitalState[0]){
		
		GetGearPosition();

		if(gearIsInPosition){
			if(gearPosition == 0){//now is in neutral
				shiftDownActive = true;
				shiftUpActive = false;
				goToNeutralActive = false;
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

	while(rawDigitalState[1]){
		shiftDownHoldingTime++;
	}

	if(shiftDownHoldingTime > SWITCHHOLDINGTIME){
		//holding shift down for long time mean go to neutral
		
		GetGearPosition();

		if(gearIsInPosition){
			if(gearPosition == 1){//in gear 1 and go to neutral
				shiftDownHoldingTime = 0;
				goToNeutralActive = true;
				shiftUpActive = false;
				shiftDownActive = false;
			}
		}else{
			problem = true;
			return;
		}

	}else{//want to shift down
		shiftDownHoldingTime = 0;
		GetGearPosition();
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
		for(shiftUpDelay = 0 ; shiftUpDelay < 50; shiftUpDelay++){
	      ;
		}

		release(GEAR_SHIFT_PORT,SHIFT_UP);
	}

	if(shiftDownActive){
		ShiftDown(gearPosition);
	}

	if(goToNeutralActive){
		GoToNeutral();
	}
	/*
	if(digIn[3] == 1){
		AutoShifting();
	}
	*/
}



int GetGearPosition(){


	int gearPositionSensorData =getGearPositionData();

	int gear;
	for(gear = 0 ; gear < 6 ; gear++){
		if (gearPositionSensorData > shiftLevelsLow[gear] && gearPositionSensorData < shiftLevelsHigh[gear]){
			gearPosition = gear;
			gearIsInPosition = true;
			break;
			}

		if (gearPositionSensorData > shiftLevelsHigh[gear] && gearPositionSensorData < shiftLevelsLow[gear+1]){
			gearPosition = gear;
			gearIsInPosition = false;
			break;
			}
	}
	gear = 0;

	if(gearIsInPosition){
		actuate(GPIOE,GPIO_Pin_1);
	}else{
		release(GPIOE,GPIO_Pin_1);
	}

	return gearPosition;



}

void ShiftDown(int gearPosition){
	int i;
	int gearPositionCurrent = 0;
  int gearPositionMonitor = 0;
	
	actuate(GEAR_SHIFT_PORT,CLUTCH);
	
	for( i=0; i< 50; i++){
		;
	}
	
	shiftDownTime = shiftDownTime + 50;

	actuate(GEAR_SHIFT_PORT,SHIFT_DOWN);


	for( ; ; gearPositionMonitor++){
		gearPositionCurrent = GetGearPosition();
		for(i = 0 ; i< 50; i++){
				;
			}
		shiftDownTime = shiftDownTime + 50;
		if(gearPositionCurrent == gearPosition - 1){
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
	int goToNeutralDelay;
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
	
	for(goToNeutralDelay = 0; goToNeutralDelay < 50; goToNeutralDelay++){
		;
	}

	release(GEAR_SHIFT_PORT,SHIFT_UP);
	release(GEAR_SHIFT_PORT,NEUTRAL);
}


void ElClutch(int elClutch){

	if(elClutch == 1){
		actuate(GPIOC,CLUTCH);
	}
	if(elClutch == 0){
		release(GPIOC,CLUTCH);
	}
}

void AutoShifting(void){
	//TODO
	//ashfaq
	//read engine speed through CAN or....????

	//int engineSpeed =
	//int vehicalSpeed = (rawAnalogState[2] + rawAnalogState[3])/2;




}
