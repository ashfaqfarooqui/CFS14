/*
 * gearshfting.c
 *
 *  Created on: Mar 17, 2014
 *      Author: Zeyang Geng
 */

#include "gearshifting.h"

/********** GLOBAL DEFINATION **********/
unsigned int gearPosition = 0;
bool gearIsInPosition = false;

const unsigned short shiftLevelsHigh[7] =
	{NEUTRALHIGH, GEAR1HIGH, GEAR2HIGH, GEAR3HIGH, GEAR4HIGH, GEAR5HIGH, GEAR6HIGH};
const unsigned short shiftLevelsLow[7] =
	{NEUTRALLOW, GEAR1LOW, GEAR2LOW, GEAR3LOW, GEAR4LOW, GEAR5LOW, GEAR6LOW};

/***************************************/


void ShiftUpActive(void){

	unsigned int sensorMonitor = 0;

	gearIsInPosition = GearIsInPosition();
	gearPosition = GetGearPosition();

	if (!gearIsInPosition){
		sensorMonitor++;
	}else{
		if (gearPosition = 0){
			//now it is in neutral
			ShiftDown(gearPosition);
		}else if (gearPosition > (MAX_GEAR_POSITION - 1)){
			//in highest gear position
			return;
			//TODO PT
			//warn driver?
		}else{
			ShiftUp(gearPosition);
		}

	}

	if (sensorMonitor > MONITOR_TIME){
		//TODO PT
		//Active clutch
		//put it into higher position
        snesorMonitor = 0;
	}

	return;
}

void ShiftDownActive(void){

	unsigned int sensorMonitor = 0;

	gearIsInPosition = GearIsInPosition();
	gearPosition = GetGearPosition();

	if (!gearIsInPosition){
		sensorMonitor++;
	}else{
		if(gearPosition < MIN_GEAR_POSITION){
			//in neutral
			return;
			//TODO PT
			//warn driver?
		}else if(gearPosition = 1){
			//now it is in first gear and driver want to go to neutral
			//TODO PT
			//warn driver?
			//GoToNeutral();
		}else{
			ShiftDown(gearPosition);
		}

	}

	if (sensorMonitor > MONITOR_TIME){
		//TODO PT
		//Active clutch
		//put it into higher position
	    snesorMonitor = 0;
	}

	return;
}

void GoToNeutralActive(void){
	unsigned int sensorMonitor = 0;

	gearIsInPosition = GearIsInPosition();
	gearPosition = GetGearPosition();

	if (!gearIsInPosition){
		sensorMonitor++;
	}else{
		if(gearPosition != 1){
			return;
			//TODO PT
			//warn driver?
		}else{
			goToNeutral();
		}
	}

	if (sensorMonitor > MONITOR_TIME){
		//TODO PT
		//Active clutch
		//put it into higher position
		snesorMonitor = 0;
	}

	return;

}

bool GearIsInPosition(){
	//uint16_t gearPositionSensorData =
	//TODO Ashfaq
	for(int i = 0 ; i < 7 ; i++){
		if (gearPositionSensorData > shiftLevelsLow[i] && gearPositionSensorData < shiftLevelsHigh[i]){
			return true;
		}
	}

	return false;
}

Int GetGearPosition(){

	//uint16_t gearPositionSensorData =
	//TODO Ashfaq
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

void ShiftUp(int gearPosition){

	//Give out shift up signal
	//TODO Ashfaq

	int gearCylinderPositionCurrent = 0;
	int gearPositionCurrent = 0;

	for(int cylinderPositionMonitor = 0 ; ; cylinderPositionMonitor++){
		gearCylinderPositionCurrent = GetGearCylinderPosition();
		//TODO Ashfaq
		delay(100);
		//TODO Ashfaq
		if (gearCylinderPositionCurrent < NATURAL_CYLINDER_POSITION){
			//cylinder start to move up
			//TODO PT
			CutIgnition();
			break;
		}

		if(cylinderPositionMonitor>MONITOR_TIME){
			//after active the cylinder doesn't move
			//TODO PT
		}
	}

	for(int gearPositionMonitor = 0 ; ; gearPositionMonitor++){
		gearPositionCurrent = GetGearPosition();
		//TODO Ashfaq
		delay(100);

		//TODO Ashfaq
		if(gearPositionCurrent = gearPosition + 1){
			RestoreIgnition();
			break;
		}

		if(cylinderPositionMonitor>MONITOR_TIME){


			//after cylinder move the gear cannot reach next level
			//TODO PT
		}
	}

	//ReleaseCylinder
	//TODO Ashfaq

	return;
}

void ShiftDown(int gearPosition){
	//Clutch();
	//TODO Ashfaq, PT

	//Wait?
	//TODO PT

	//give out shift down signal
	//TODO Ashfaq

	int gearPositionCurrent = 0;

	for(int gearPositionMonitor = 0 ; ; gearPositionMonitor++){
		gearPositionCurrent = GetGearPosition();
		//TODO Ashfaq
		delay(100);

		//TODO Ashfaq
		if(gearPositionCurrent = gearPosition - 1){
			//clutch back;
			//TODO PT
			break;
		}
	}


	//release gear cylinder
	//release clutch cylinder
	//same time or wait?
	//TODO PT

	return;
}

void GoToNeutral(void){
	//give out neutral cylinder signal
	//TODO Ashfaq

	//how can we know that the neutral cylinder is in position?
	//Or wait for some time to let it in position?
	//what if this one failed
	//TODO Toni

	//give out shiftup signal
	//TODO Ashfaq

	int gearPositionCurrent = 0;

	for(int gearPositionMonitor = 0 ; ; gearPositionMonitor++){
		gearPositionCurrent = GetGearPosition();
		//TODO Ashfaq
		delay(100);
		//TODO Ashfaq
		if(gearPositionCurrent = 0){
			//release gear
		}

	}
}

void CutIgnition(void){

}

void RestoreIgnition(void){

}

void ElClutch(void){

}

uint16_t GetGearCylinderPosition(void){

}

uint16_t GetGearPositionSensorData(void){

}
