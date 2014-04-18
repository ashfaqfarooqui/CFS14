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
	//TODO Update switch actions
}
