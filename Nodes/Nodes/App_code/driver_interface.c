#include "driver_interface.h"

static uint8_t previousSwitchStates = 0x00;
static uint8_t RawSwitchStates = 0x00;

short debounce[10] = {
0
};

unsigned int rawDigitalState[NUMBER_OF_DIGITAL_IN_PER_NODE]={0};

/** initialize the features of the driver interface*/
void init_driverInterface()
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	if (THIS_NODE == FRONT_NODE)
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

	} else if (THIS_NODE == REAR_NODE)
	{
		GPIO_InitStructure.GPIO_Pin =
		GEARUP | GEARDOWN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);

	}

	for(i=0;i<NUMBER_OF_DIGITAL_IN_PER_NODE;i++){
		rawDigitalState[i]=0;
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
void updateSwitches()
{
	uint8_t digIn[NUMBER_OF_DIGITAL_IN_PER_NODE],i;
	digIn[0] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, GEARUP),
			GEARUP_POS, FAST_SWITCH_DELAY);
	digIn[1] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, GEARDOWN),
			GEARDOWN_POS , FAST_SWITCH_DELAY);
	if (THIS_NODE == FRONT_NODE)
	{
		digIn[2] = debounceInput(
				!GPIO_ReadInputDataBit(INPUTPORT, LAUNCH_CONTROL), LC_POS ,
				SLOW_SWITCH_DELAY);
		digIn[3] = debounceInput(
				!GPIO_ReadInputDataBit(INPUTPORT, TRACTION_CONTROL), TC_POS ,
				SLOW_SWITCH_DELAY);
		digIn[4] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, DATALOGGER),
				DL_POS , SLOW_SWITCH_DELAY);
		digIn[5] = debounceInput(
				!GPIO_ReadInputDataBit(INPUTPORT, AUTOSHIFTING), AS_POS ,
				SLOW_SWITCH_DELAY);
		digIn[6] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, FANCONTROL),
				FC_POS , SLOW_SWITCH_DELAY);
		digIn[7] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, E_CLUTCH),
				EC_POS , SLOW_SWITCH_DELAY);
	} else
	{
		digIn[2] = 0;
		digIn[3] = 0;
		digIn[4] = 0;
		digIn[5] = 0;
	}
	digIn[8] = 0;
	digIn[9] = 0;
	for (i = 0; i < NUMBER_OF_DIGITAL_IN_PER_NODE; i++)
	{
		*(rawDigitalState + i) = digIn[i];
	}
}
void ReadSwitchStates()
{
	RawSwitchStates |= (GPIO_ReadInputDataBit(INPUTPORT, GEARUP) << GEARUP_POS)
			| (GPIO_ReadInputDataBit(INPUTPORT, GEARDOWN) << GEARDOWN_POS)
			| (GPIO_ReadInputDataBit(INPUTPORT, TRACTION_CONTROL) << TC_POS)
			| (GPIO_ReadInputDataBit(INPUTPORT, DATALOGGER) << DL_POS)
			| (GPIO_ReadInputDataBit(INPUTPORT, AUTOSHIFTING) << AS_POS)
			| (GPIO_ReadInputDataBit(INPUTPORT, FANCONTROL) << FC_POS)
			| (GPIO_ReadInputDataBit(INPUTPORT, E_CLUTCH) << EC_POS)
			| (GPIO_ReadInputDataBit(INPUTPORT, LAUNCH_CONTROL) << LC_POS);

	if ((RawSwitchStates & (1 << GEARUP_POS)) == 1 << GEARUP_POS) //check if gear up is triggerd
	{
//TODO GearUp algo
		RawSwitchStates = RawSwitchStates & 1 << GEARUP_POS; //clear GREARUP flag
	} else if ((RawSwitchStates & 1 << GEARDOWN_POS) == 1 << GEARDOWN_POS) //check if gearDown is triggered
	{
//TODO GearDown algo
		RawSwitchStates = RawSwitchStates & 1 << GEARDOWN_POS; //Clear Gear down flag
	} else if (RawSwitchStates & (1 << EC_POS) != 1 << EC_POS)
	{
		//TODO: turnOnn el-clutch control
	} else if (previousSwitchStates != RawSwitchStates)
	{
//TODO do the required control
	}
	previousSwitchStates = RawSwitchStates;
}
void updateSwitchStates()
{

}
void setSwitchStates(uint8_t states)
{
	RawSwitchStates = states;
}

unsigned char debounceInput(int buttonValue, int stateIndex, int debounceDelay)
{

	//NOTHING TO DO
	if (!buttonValue && !rawDigitalState[stateIndex])
	{
		debounce[stateIndex] = 0;
		return 0;
	}

	// ACTIVATE
	if (buttonValue)
	{
		if (debounce[stateIndex] >= debounceDelay)
		{
			return 1;
		} else
		{
			debounce[stateIndex]++;
			return 0;
		}
	}

	// DEACTIVATE
	if (!buttonValue && rawDigitalState[stateIndex])
	{
		if (debounce[stateIndex] <= 0)
		{
			return 0;
		} else
		{
			debounce[stateIndex]--;
			return 1;
		}
	}

	return rawDigitalState[stateIndex];
}

void switchAction()
{
	if (RawSwitchStates & (1 << TC_POS) == 1 << TC_POS)
	{
//TODO: turnOff Tranction control
	}
	if (RawSwitchStates & (1 << TC_POS) != 1 << TC_POS)
	{
//TODO: turnOn Tranction control
	}

	if (RawSwitchStates & (1 << LC_POS) == 1 << LC_POS)
	{
		//TODO: turnOf launch control
	}
	if (RawSwitchStates & (1 << LC_POS) != 1 << LC_POS)
	{
		//TODO: turnOn launch control
	}
	if (RawSwitchStates & (1 << DL_POS) == 1 << DL_POS)
	{
		//TODO: turnOf datalogger control
	}
	if (RawSwitchStates & (1 << DL_POS) != 1 << DL_POS)
	{
		//TODO: turnOn Datalogger control
	}

	if (RawSwitchStates & (1 << FC_POS) == 1 << FC_POS)
	{
		//TODO: Tunr fan on
	}
	if (RawSwitchStates & (1 << FC_POS) != 1 << FC_POS)
	{
		//TODO: Connect FAN to auto control mode
	}
}
