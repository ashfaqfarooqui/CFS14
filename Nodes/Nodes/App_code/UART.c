#include "UART.h"
#define NUMBER_OF_BYTES 12
uint8_t RxBuffer[NUMBER_OF_BYTES]={0,0,0,0,0,0,0,0,0,0,0,0};
void init_uart(uint32_t baudrate)
{

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_Init(UART4, &USART_InitStructure);

	UART_DMA_Config();
	  USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(UART4, ENABLE);


}

void UART_DMA_Config()
{
	DMA_InitTypeDef  DMA_InitStructure;

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &RxBuffer[0];
	DMA_InitStructure.DMA_BufferSize = NUMBER_OF_BYTES;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(UART4->DR));
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream2, ENABLE);
}
void USART_puts_char(USART_TypeDef* USARTx, char *s)
{
	while (*s)
	{
		while ((USART_GetFlagStatus(UART4, USART_FLAG_TXE) == SET))
		{
			USART_SendData(UART4, *s++);
			
		}
	}
	
}

void USART_puts_int8(USART_TypeDef* USARTx, uint8_t *s)
{
	while (*s)
	{
		while ((USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET))
		{
			USART_SendData(UART4, *s++);

		}
	}

}

void USART_puts_int16(USART_TypeDef* USARTx, uint16_t *s)
{
	while (*s)
	{
		while ((USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET))
		{
			USART_SendData(UART4, *s++);

		}
	}

}
uint8_t* getRecievedData()
{
	return RxBuffer;
}
