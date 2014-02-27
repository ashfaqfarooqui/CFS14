#include "driver_interface.h"


/** initialize the features of the driver interface*/
void init_driverInterface(uint8_t node) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	if (node == FRONT_NODE) {
		//Inputs
		GPIO_InitStructure.GPIO_Pin = GEARUP | GEARDOWN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);

		//outputs
		GPIO_InitStructure.GPIO_Pin = WARNINGLIGHT;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(OUTPUTPORT, &GPIO_InitStructure);

	} else if (node == REAR_NODE) {
		GPIO_InitStructure.GPIO_Pin = GEARUP | GEARDOWN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);

	}
}

void SwitchWarningLight(uint8_t ONOFF) {
	if (ONOFF == ON) {
		GPIO_SetBits(OUTPUTPORT, WARNINGLIGHT);
	}else if(ONOFF == OFF){
		GPIO_ResetBits(OUTPUTPORT,WARNINGLIGHT);
	}
}

void ReadSwitchStates()
{
	SwitchState=SwitchState;///////////;
	if((SwitchState & 1<<GEARUPPOS)==1<<GEARUPPOS)
	{
		//TODO GearUp algo
		SwitchState=SwitchState&1<<GEARUPPOS;//clear GREARUP flag
	}
	else if((SwitchState & 1<<GEARDOWNPOS)==1<<GEARDOWNPOS)
	{
		//TODO GearDown algo
		SwitchState=SwitchState&1<<GEARDOWNPOS; //Clear Gear down flag
	}
	else if(previousSwitchState!=SwitchStates)
	{
		//TODO do the required control
	}
	previousSwitchState=SwitchState;
}
