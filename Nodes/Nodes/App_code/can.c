/*
 * can.c
 *
 *  Created on: 18-Jan-2014
 *      Author: Ashfaq
 */

#include "can.h"

uint8_t TransmitMailbox = 0;
bol FIFOReleased =FALSE;

void CAN_configureFilter(uint8_t CAN_FilterNumber, uint8_t CAN_FilterMode,
		uint8_t CAN_FilterScale, uint16_t CAN_FilterIdHigh,
		uint16_t CAN_FilterIdLow, uint16_t CAN_FilterMaskIdHigh,
		uint16_t CAN_FilterMaskIdLow, uint16_t CAN_FilterFIFOAssignment,
		FunctionalState CAN_FilterActivation)
{
	/* CAN filter init */
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber = CAN_FilterNumber;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale;
	CAN_FilterInitStructure.CAN_FilterIdHigh = CAN_FilterIdHigh;
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_FilterIdLow;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = CAN_FilterMaskIdHigh;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_FilterMaskIdLow;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFOAssignment;
	CAN_FilterInitStructure.CAN_FilterActivation = CAN_FilterActivation;
	CAN_FilterInit(&CAN_FilterInitStructure);
}

void init_CAN_Communication()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;

	/* CAN GPIOs configuration **************************************************/

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
	GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT);

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

	/* CAN configuration ********************************************************/
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/* CAN register init */
	CAN_DeInit(CANx);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = ENABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

	/* CAN Baudrate = 500KbBps (CAN clocked at 42 MHz) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
	CAN_InitStructure.CAN_Prescaler = 4;
	if (CAN_Init(CANx, &CAN_InitStructure) == CAN_InitStatus_Success)
	{
		STM_EVAL_LEDOn(LED_GREEN);
	}
//	NVIC_Config_CAN();
	CAN_ReceiverInit(&RxMessage);

}

CanTxMsg CAN_createMessage_uint(uint32_t StdId, uint8_t RTR, uint8_t IDE,
		uint8_t DLC, uint8_t *data)
{
	int ctr;
	/* transmit */
	CanTxMsg TxMessage;
	TxMessage.StdId = StdId;
	TxMessage.RTR = RTR;
	TxMessage.IDE = IDE;
	TxMessage.DLC = DLC;
	for (ctr = 0; ctr < DLC; ctr++)
	{
		TxMessage.Data[ctr] = *(data + ctr);
	}

	return TxMessage;
}

CanTxMsg CAN_createMessage_int(uint32_t StdId, uint8_t RTR, uint8_t IDE,
		uint8_t DLC, int *data)
{
	int ctr;
	/* transmit */
	CanTxMsg TxMessage;
	TxMessage.StdId = StdId;
	TxMessage.RTR = RTR;
	TxMessage.IDE = IDE;
	TxMessage.DLC = DLC;
	for (ctr = 0; ctr < DLC; ctr++)
	{
		TxMessage.Data[ctr] = *(data + ctr);
	}

	return TxMessage;
}

void CAN_ReceiverInit(CanRxMsg *RxMessage)
{
	/* receive */

	RxMessage->StdId = 0x00;
	RxMessage->IDE = CAN_ID_STD;
	RxMessage->DLC = 0;
	RxMessage->Data[0] = 0x00;
	RxMessage->Data[1] = 0x00;
	RxMessage->Data[2] = 0x00;
	RxMessage->Data[3] = 0x00;
	RxMessage->Data[4] = 0x00;
	RxMessage->Data[5] = 0x00;
	RxMessage->Data[6] = 0x00;
	RxMessage->Data[7] = 0x00;
}

void CAN_transmit_data(CanTxMsg TxMessage)
{

	TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);

}

void NVIC_Config_CAN()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

CanRxMsg* getRXmsg()
{
	return &RxMessage;
}
void readMessages()
{
	FIFOReleased = FALSE;
	if (CAN_GetFlagStatus(CAN1, CAN_FLAG_FMP0) == SET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		if (RxMessage.StdId == CAN_ID_ENGINE_RPM)
		{
			sensorData[ENGINE_RPM] = 0;
			sensorData[ENGINE_RPM] |= RxMessage.Data[3];
			sensorData[ENGINE_RPM] = (sensorData[ENGINE_RPM] << 8)
					| RxMessage.Data[2];
			CAN_FIFORelease(CAN1, CAN_FIFO0);
			FIFOReleased = TRUE;
		}
		if (RxMessage.StdId == CAN_ID_COOLANT_TEMP)
		{
			sensorData[WATER_TEMPRATURE] = 0;
			sensorData[WATER_TEMPRATURE] |= RxMessage.Data[3];
			sensorData[WATER_TEMPRATURE] = (sensorData[WATER_TEMPRATURE] << 8)
					| RxMessage.Data[2];
			CAN_FIFORelease(CAN1, CAN_FIFO0);
			FIFOReleased = TRUE;
		}
		if (RxMessage.StdId == CAN_ID_RPM)
		{
			sensorData[1] = 0;
			sensorData[1] |= RxMessage.Data[3];
			sensorData[1] = (sensorData[1] << 8) | RxMessage.Data[2];
			CAN_FIFORelease(CAN1, CAN_FIFO0);
			FIFOReleased = TRUE;
		}
		if (THIS_NODE == REAR_NODE && RxMessage.StdId == CAN_ID_SWITCH_STATES)
		{
			recievedStates = RxMessage.Data[0];
			CAN_FIFORelease(CAN1, CAN_FIFO0);
			FIFOReleased = TRUE;
		}
		if (THIS_NODE == FRONT_NODE
				&& RxMessage.StdId == CAN_ADR_WATER_TEMPRATURE)
		{
			sensorData[WATER_TEMPRATURE] = RxMessage.Data[1];
			sensorData[WATER_TEMPRATURE] |= (RxMessage.Data[2] << 8);
			CAN_FIFORelease(CAN1, CAN_FIFO0);
			FIFOReleased = TRUE;
		}
		if (FIFOReleased == FALSE)
		{
			CAN_FIFORelease(CAN1, CAN_FIFO0);
		}

	}
}

void CAN1_RX0_IRQHandler()
{
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		STM_EVAL_LEDToggle(LED_BLUE);
		CAN_FIFORelease(CAN1, CAN_FIFO0);
	}
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}
