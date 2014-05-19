#include "driver_interface.h"
unsigned int recievedStates = 0;
uint8_t previousRecievedStates = 0;
uint8_t previousSwitchStates = 0x00;
bol AutoShiftState = FALSE;
bol TractionControlState = FALSE;
short debounce[10] = {
0
};

unsigned int rawDigitalState[NUMBER_OF_DIGITAL_IN_PER_NODE] = {
0
};

/** initialize the features of the driver interface*/
void init_driverInterface()
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	if (THIS_NODE == FRONT_NODE)
	{
//Inputs
		GPIO_InitStructure.GPIO_Pin =
		TRACTION_CONTROL  | AUTOSHIFTING | DATALOGGER;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = E_CLUTCH;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = LAUNCH_CONTROL;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = FANCONTROL;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

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
		//inputs
		GPIO_InitStructure.GPIO_Pin = GEARDOWN; //make gear up pd12
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GEARUP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

	}

	for (i = 0; i < NUMBER_OF_DIGITAL_IN_PER_NODE; i++)
	{
		rawDigitalState[i] = 0;
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
	uint8_t digIn[NUMBER_OF_DIGITAL_IN_PER_NODE], i;
	if (THIS_NODE == FRONT_NODE)
	{

		digIn[2] = debounceInput(
				!GPIO_ReadInputDataBit(GPIOD, LAUNCH_CONTROL), LC_POS,
				SLOW_SWITCH_DELAY);
		digIn[3] = debounceInput(
				!GPIO_ReadInputDataBit(INPUTPORT, TRACTION_CONTROL), TC_POS,
				SLOW_SWITCH_DELAY);
		digIn[4] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, DATALOGGER),
		DL_POS, SLOW_SWITCH_DELAY);
		digIn[5] = debounceInput(
				!GPIO_ReadInputDataBit(INPUTPORT, AUTOSHIFTING), AS_POS,
				SLOW_SWITCH_DELAY);
//		digIn[6] = debounceInput(!GPIO_ReadInputDataBit(GPIOD, FANCONTROL),
//		FC_POS, SLOW_SWITCH_DELAY);
		digIn[7] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, E_CLUTCH),
		EC_POS, SLOW_SWITCH_DELAY);
	}
	if (THIS_NODE == REAR_NODE)
	{
		digIn[0] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, GEARUP),
		GEARUP_POS, FAST_SWITCH_DELAY);
		digIn[1] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, GEARDOWN),
		GEARDOWN_POS, FAST_SWITCH_DELAY);
	}

	for (i = 0; i < NUMBER_OF_DIGITAL_IN_PER_NODE; i++)
	{
		*(rawDigitalState + i) = digIn[i];
	}
	switchAction();
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
	if (THIS_NODE == FRONT_NODE)
	{
		uint8_t switchState = 0;
		CanTxMsg switchStatesMsg;
		uint8_t transmitStatus;
		if (rawDigitalState[2] == SET)
		{
			switchState = switchState | 0x01; //LC-ON
		} else if (rawDigitalState[2] == RESET)
		{
			switchState = switchState & 0xFE; //LC-OFF
		}
		if (rawDigitalState[3] == SET)
		{
			switchState = switchState | 0x02; //TC-ON
		} else if (rawDigitalState[3] == RESET)
		{
			switchState = switchState & 0xFD; //TC-off
		}
		if (rawDigitalState[4] == SET)
		{
			switchState = switchState | 0x04; //DL-ON
		} else if (rawDigitalState[4] == RESET)
		{
			switchState = switchState & 0xFB; //DL-OFF
		}
		if (rawDigitalState[5] == SET)
		{
			switchState = switchState | 0x08; //AS-OFF
		} else if (rawDigitalState[5] == RESET)
		{
			switchState = switchState | 0xF7; //AS-OFF
		}
//		if (rawDigitalState[5] == SET)
//		{
//			switchState = switchState | 0x10; //FC-OFF
//		} else if (rawDigitalState[5] == RESET)
//		{
//			switchState = switchState | 0xEF; //FC-OFF
//		}

		switchStatesMsg = CAN_createMessage_uint(CAN_ID_SWITCH_STATES,
		CAN_RTR_DATA, CAN_ID_STD, 1, &switchState);
		if (previousSwitchStates != switchState)
		{
			do
			{
				transmitStatus = CAN_Transmit(CAN1, &switchStatesMsg);
			} while (transmitStatus == CAN_TxStatus_NoMailBox);
			previousSwitchStates = switchState;
		}
	}
	if (THIS_NODE == REAR_NODE)
	{
		if (previousRecievedStates != recievedStates)
		{

			if ((recievedStates & 0x01) == 0x01)
			{
				//LC-ON
			} else
			{
				//LC-OFF
			}

			if ((recievedStates & 0x02) == 0x02)
			{
				TractionControlState = TRUE; //TC-ON
			} else
			{
				TractionControlState = FALSE; //TC-OFF
			}

			if ((recievedStates & 0x08) == 0x08)
			{
				AutoShiftState = TRUE; //AS-ON
			} else
			{
				AutoShiftState = FALSE; //AS_OFF
			}
//			if ((recievedStates & 0x10) == 0x10)
//			{
//				setFanSpeed(0);//ON
//			} else
//			{
//				setFanSpeed(100);//Off
//			}
			//TODO actuate based on  switchData
		}
	}
}
