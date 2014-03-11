#include "driver_interface.h"

static uint8_t previousSwitchStates = 0x00;
static uint8_t SwitchStates = 0x00;
/** initialize the features of the driver interface*/
void init_driverInterface(uint8_t node)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	if (node == FRONT_NODE)
	{
//Inputs
		GPIO_InitStructure.GPIO_Pin =
		GEARUP | GEARDOWN | TRACTION_CONTROL | LAUNCH_CONTROL | E_CLUTCH
				| FANCONTROL | AUTOSHIFTING | DATALOGGER;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);

//outputs
		GPIO_InitStructure.GPIO_Pin =
		WARNINGLIGHT;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(OUTPUTPORT, &GPIO_InitStructure);

	} else if (node == REAR_NODE)
	{
		GPIO_InitStructure.GPIO_Pin =
		GEARUP | GEARDOWN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);

	}

}

void SwitchWarningLight(uint8_t ONOFF)
{
	if (ONOFF == ON)
	{
		GPIO_SetBits(OUTPUTPORT,
		WARNINGLIGHT);
	} else if (ONOFF == OFF)
	{
		GPIO_ResetBits(OUTPUTPORT,
		WARNINGLIGHT);
	}
}

void ReadSwitchStates()
{
	SwitchStates |= (GPIO_ReadInputDataBit(GEARUP) << GEARUP_POS)
			| (GPIO_ReadInputDataBit(GEARDOWN) << GEARDOWN_POS)
			| (GPIO_ReadInputDataBit(TRACTION_CONTROL) << TC_POS)
			| (GPIO_ReadInputDataBit(DATALOGGER) << DL_POS)
			| (GPIO_ReadInputDataBit(AUTOSHIFTING) << AS_POS)
			| (GPIO_ReadInputDataBit(FANCONTROL) << FC_POS)
			| (GPIO_ReadInputDataBit(E_CLUTCH) << EC_POS)
			| (GPIO_ReadInputDataBit(LAUNCH_CONTROL) << LC_POS);

	if ((SwitchStates & (1 << GEARUP_POS)) == 1 << GEARUP_POS) //check if gear upis triggerd
	{
//TODO GearUp algo
		SwitchStates = SwitchStates & 1 << GEARUP_POS; //clear GREARUP flag
	} else if ((SwitchStates & 1 << GEARDOWN_POS) == 1 << GEARDOWN_POS) //check if gearDown is triggered
	{
//TODO GearDown algo
		SwitchStates = SwitchStates & 1 << GEARDOWN_POS; //Clear Gear down flag
	} else if (previousSwitchStates != SwitchStates)
	{
//TODO do the required control
	}
	previousSwitchStates = SwitchStates;
}
void setSwitchStates(uint8_t states)
{
	SwitchStates = states;
}

void switchAction()
{
	if (SwitchStates & (1 << TC_POS) == 1 << TC_POS)
	{
//TODO: turnOff Tranction control
	}
	if (SwitchStates & (1 << TC_POS) != 1 << TC_POS)
	{
//TODO: turnOn Tranction control
	}

	if (SwitchStates & (1 << LC_POS) == 1 << LC_POS)
	{
		//TODO: turnOf launch control
	}
	if (SwitchStates & (1 << LC_POS) != 1 << LC_POS)
	{
		//TODO: turnOn launch control
	}
	if (SwitchStates & (1 << DL_POS) == 1 << DL_POS)
	{
		//TODO: turnOf datalogger control
	}
	if (SwitchStates & (1 << DL_POS) != 1 << DL_POS)
	{
		//TODO: turnOn Datalogger control
	}

	if (SwitchStates & (1 << EC_POS) == 1 << EC_POS)
	{
		//TODO: turnOf el-clutch control
	}
	if (SwitchStates & (1 << EC_POS) != 1 << EC_POS)
	{
		//TODO: turnOn el-clutch control
	}
	if (SwitchStates & (1 << FC_POS) == 1 << FC_POS)
	{
		//TODO: Tunr fan on
	}
	if (SwitchStates & (1 << FC_POS) != 1 << FC_POS)
	{
		//TODO: Connect FAN to auto control mode
	}
}
