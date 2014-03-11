/*
 * can.c
 *
 *  Created on: 18-Jan-2014
 *      Author: Ashfaq
 */

#include "can.h"

uint8_t TransmitMailbox = 0;

void CAN_configureFilter(uint8_t CAN_FilterNumber, uint8_t CAN_FilterMode,
		uint8_t CAN_FilterScale, uint16_t CAN_FilterIdHigh,
		uint16_t CAN_FilterIdLow, uint16_t CAN_FilterMaskIdHigh,
		uint16_t CAN_FilterMaskIdLow, uint16_t CAN_FilterFIFOAssignment,
		FunctionalState CAN_FilterActivation) {
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

void init_CAN_Communication() {
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
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

	/* CAN Baudrate = 1 MBps (CAN clocked at 30 MHz) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 2;
	CAN_Init(CANx, &CAN_InitStructure);
	NVIC_Config_CAN();

}

CanTxMsg CAN_createMessage(uint32_t StdId, uint8_t RTR, uint8_t IDE,
		uint8_t DLC, uint8_t *data) {
	int ctr;
	/* transmit */
	CanTxMsg TxMessage;
	TxMessage.StdId = StdId;
	TxMessage.RTR = RTR;
	TxMessage.IDE = IDE;
	TxMessage.DLC = DLC;
	for (ctr = 0; ctr < DLC; ctr++) {
		TxMessage.Data[ctr] = *(data + ctr);
	}

	return TxMessage;
}

void CAN_ReceiverInit(CanRxMsg *RxMessage) {
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

void CAN_transmit_data(CanTxMsg TxMessage) {

	TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);

}
/*
 void CAN_DMA_config()
 {
 DMA_InitTypeDef DMA_initStructure;


 DMA_initStructure.DMA_Channel = DMA_Channel_;
 DMA_initStructure.DMA_Memory0BaseAddr = (uint32_t)screenBuffer;
 DMA_initStructure.DMA_PeripheralBaseAddr = (uint32_t)(); //CAN DR
 DMA_initStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 DMA_initStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
 DMA_initStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
 DMA_initStructure.DMA_Mode = DMA_Mode_Normal;
 DMA_initStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 DMA_initStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 DMA_initStructure.DMA_BufferSize = 6*84;
 DMA_initStructure.DMA_Priority = DMA_Priority_High;
 DMA_initStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 DMA_initStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 DMA_initStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

 DMA_Init(DMA1_Stream4, &DMA_initStructure);
 DMA_Cmd(DMA1_Stream4, ENABLE);

 SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
 }
 */
void NVIC_Config_CAN() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN1_RX0_IRQHandler() {
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
		CAN_Receive(CAN1, CAN_FIFO0, RxMessage);
		STM_EVAL_LEDToggle(LED_BLUE);
	}
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
}
