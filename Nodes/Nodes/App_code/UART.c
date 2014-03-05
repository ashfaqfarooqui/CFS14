
#include "UART.h"


void init_uart(uint32_t baudrate){

USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_Init(GPIOC, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);


USART_InitStructure.USART_BaudRate=baudrate;
USART_InitStructure.USART_WordLength=USART_WordLength_8b;
USART_InitStructure.USART_StopBits=USART_StopBits_2;
USART_InitStructure.USART_Parity=USART_Parity_No;
USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
USART_Init(UART4,&USART_InitStructure);

USART_Cmd(UART4,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel =UART4_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

}


void USART_puts_char(USART_TypeDef* USARTx, char *s){
	while(*s){
		while((USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET))
		{
			USART_SendData(UART4,*s++);
			
		}
	}
	
}

void USART_puts_int8(USART_TypeDef* USARTx, uint8_t *s){
	while(*s){
		while((USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET))
		{
			USART_SendData(UART4,*s++);

		}
	}

}

void USART_puts_int16(USART_TypeDef* USARTx, uint16_t *s){
	while(*s){
		while((USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET))
		{
			USART_SendData(UART4,*s++);

		}
	}

}
