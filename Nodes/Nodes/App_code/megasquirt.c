/*
 * megasquirt.c
 *
 *  Created on: May 16, 2014
 *      Author: Zeyang Geng
 */
#include "megasquirt.h"

/********** GLOBAL DEFINATION **********/

/***************************************/

void launchControl()
{
	int kp = 25;
	int kd = 25;
	int error;
	if (LaunchControlActivated == TRUE)
	{

		ElClutch((ElectricClutchActuated==TRUE) || (LaunchControlActivated==TRUE));
		error = sensorData[ENGINE_RPM] - SET_RPM;
		//if (error > 400)
		//{
			cutIgnition(0);
		//}

	} else
	{
		cutIgnition(PERIOD_GEAR);
	}
}
