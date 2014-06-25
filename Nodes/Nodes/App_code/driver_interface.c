#include "driver_interface.h"
unsigned int recievedStates = 0;
uint8_t previousRecievedStates = 0;
uint8_t previousSwitchStates = 0x00;
bol AutoShiftState = FALSE;
bol TractionControlState = FALSE;
bol ElectricClutchActuated = FALSE;
bol LaunchControlActivated = FALSE;
short debounce[10] = {
0
};

unsigned int rawDigitalState[NUMBER_OF_DIGITAL_IN_PER_NODE] = {
0
};

/** initialize the features of the driver interface*/
uint8_t DLdata[];
char ecldata;
void init_driverInterface()
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	if (THIS_NODE == FRONT_NODE)
	{
//Inputs
		GPIO_InitStructure.GPIO_Pin =
		TRACTION_CONTROL | AUTOSHIFTING | DATALOGGER;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = LAUNCH_CONTROL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

//		GPIO_InitStructure.GPIO_Pin = DATALOGGER;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);

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
		GPIO_InitStructure.GPIO_Pin = GEARDOWN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_Init(INPUTPORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = E_CLUTCH;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		
		/*
		 GPIO_InitStructure.GPIO_Pin =
		 TRACTION_CONTROL | AUTOSHIFTING | DATALOGGER;
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		 GPIO_Init(INPUTPORT, &GPIO_InitStructure);
		 */

	}

	for (i = 0; i < NUMBER_OF_DIGITAL_IN_PER_NODE; i++)
	{
		rawDigitalState[i] = 0;
	}
}

void switchWarningLight(bol ONOFF)
{
	if (ONOFF == TRUE)
	{
		GPIO_SetBits(OUTPUTPORT,
		WARNINGLIGHT);
	} else if (ONOFF == FALSE)
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

		digIn[2] = debounceInput(!GPIO_ReadInputDataBit(GPIOD, LAUNCH_CONTROL),
		LC_POS, SLOW_SWITCH_DELAY);
		digIn[3] = debounceInput(
				!GPIO_ReadInputDataBit(INPUTPORT, TRACTION_CONTROL),
				TC_POS, SLOW_SWITCH_DELAY);
		digIn[4] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, DATALOGGER),
		DL_POS, SLOW_SWITCH_DELAY);
		digIn[5] = debounceInput(
				!GPIO_ReadInputDataBit(INPUTPORT, AUTOSHIFTING),
				AS_POS, SLOW_SWITCH_DELAY);
//		digIn[6] = debounceInput(!GPIO_ReadInputDataBit(GPIOD, FANCONTROL),
//		FC_POS, SLOW_SWITCH_DELAY);
//		digIn[7] = debounceInput(!GPIO_ReadInputDataBit(GPIOE, E_CLUTCH),
//		EC_POS, MEDIUM_SWITCH_DELAY);
	}
	if (THIS_NODE == REAR_NODE)
	{
		//digIn[0] = debounceInput(!GPIO_ReadInputDataBit(GPIOD, GEARUP),
		//GEARUP_POS, FAST_SWITCH_DELAY);
		digIn[0] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, GPIO_Pin_8),
		DL_POS, FAST_SWITCH_DELAY);
		digIn[1] = debounceInput(!GPIO_ReadInputDataBit(INPUTPORT, GEARDOWN),
		GEARDOWN_POS, FAST_SWITCH_DELAY);
		digIn[7] = debounceInput(!GPIO_ReadInputDataBit(GPIOE, E_CLUTCH),
		EC_POS, MEDIUM_SWITCH_DELAY);
		
//		if(digIn[7] == 1){
//			ElClutchActuated = TRUE;
//		}else{
//			ElClutchActuated = FALSE;
//		}
	}

	for (i = 0; i < NUMBER_OF_DIGITAL_IN_PER_NODE; i++)
	{
		*(rawDigitalState + i) = digIn[i];
	}
	if(rawDigitalState[GEARDOWN_POS] || rawDigitalState[GEARUP_POS])
	{
		startTimer();
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
	if (THIS_NODE == FRONT_NODE)
	{
		uint8_t switchState = 0, LctrlData;
		CanTxMsg switchStatesMsg;
		CanTxMsg DLON,  Lctrl;
		uint8_t transmitStatus;
		if (rawDigitalState[5] == SET)
		{
//			switchState = switchState | 0x01; //LC-ON
//			switchState = switchState | 0x10;
			LctrlData = 0xff;
			Lctrl = CAN_createMessage_uint(LAUNCH_BUTTON_ID,
			CAN_RTR_DATA, CAN_ID_STD, 1, &LctrlData);
			do
			{
				transmitStatus = CAN_Transmit(CAN1, &Lctrl);
			} while (transmitStatus == CAN_TxStatus_NoMailBox);

		} else if (rawDigitalState[5] == RESET)
		{
////			switchState = switchState & 0xFE; //LC-OFF
//			switchState = switchState & 0xEF;
			LctrlData = 0x11;
			Lctrl = CAN_createMessage_uint(LAUNCH_BUTTON_ID,
			CAN_RTR_DATA, CAN_ID_STD, 1, &LctrlData);
			do
			{
				transmitStatus = CAN_Transmit(CAN1, &Lctrl);
			} while (transmitStatus == CAN_TxStatus_NoMailBox);

		}
		if (rawDigitalState[TC_POS] == SET)
		{
			switchState = switchState | 0x02; //TC-ON
		} else if (rawDigitalState[TC_POS] == RESET)
		{
			switchState = switchState & 0xFD; //TC-off
		}
		if (rawDigitalState[DL_POS] == SET)
		{
			DLdata[0] = 0xff; //DL-ON
			DLON = CAN_createMessage_uint(0x500,
			CAN_RTR_DATA, CAN_ID_STD, 1, &DLdata[0]);
			do
			{
				transmitStatus = CAN_Transmit(CAN1, &DLON);
			} while (transmitStatus == CAN_TxStatus_NoMailBox);

		} else if (rawDigitalState[DL_POS] == RESET)
		{
			DLdata[0] = 0x00; //DL-Off
			DLON = CAN_createMessage_uint(0x500,
			CAN_RTR_DATA, CAN_ID_STD, 1, &DLdata[0]);
			do
			{
				transmitStatus = CAN_Transmit(CAN1, &DLON);
			} while (transmitStatus == CAN_TxStatus_NoMailBox);
		}
		if (rawDigitalState[AS_POS] == SET)
		{
			switchState = switchState | 0x08; //AS-OFF
		} else if (rawDigitalState[AS_POS] == RESET)
		{
			switchState = switchState & 0xF7; //AS-OFF
		}
		if (rawDigitalState[EC_POS] == SET)
		{
//			switchState = switchState | 0x10; //EC-OFF
		} else if (rawDigitalState[EC_POS] == RESET)
		{
//			switchState = switchState & 0xEF; //EC-OFF
		}

		//if (previousSwitchStates != switchState)
		//{
		switchStatesMsg = CAN_createMessage_uint(CAN_ID_SWITCH_STATES,
		CAN_RTR_DATA, CAN_ID_STD, 1, &switchState);
		do
		{
			transmitStatus = CAN_Transmit(CAN1, &switchStatesMsg);
		} while (transmitStatus == CAN_TxStatus_NoMailBox);
		previousSwitchStates = switchState;
		//}
	} else if (THIS_NODE == REAR_NODE)
	{
		//if (previousRecievedStates != recievedStates)
		//{

//		if ((recievedStates & 0x01) == 0x01)
//		{
//			LaunchControlActivated = TRUE;		//LC-ON
//		} else
//		{
//			LaunchControlActivated = FALSE;		//LC-OFF
//		}

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
		if (rawDigitalState[EC_POS] == SET)
		{
			ElectricClutchActuated = TRUE;
			//			switchState = switchState | 0x10; //EC-OFF
		} else if (rawDigitalState[EC_POS] == RESET)
		{
			ElectricClutchActuated = FALSE;
			//			switchState = switchState & 0xEF; //EC-OFF
		}
		//TODO actuate based on  switchData
		previousRecievedStates = recievedStates;
		//}
	}
}
