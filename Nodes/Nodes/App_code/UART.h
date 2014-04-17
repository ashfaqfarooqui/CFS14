
#ifndef __STM32F4XX_USART_H_
#define __STM32F4XX_USART_H_
#include "stm32f4xx_usart.h"
#include "stm32f4xx_DMA.h"

void init_uart(uint32_t );
void USART_puts_char(USART_TypeDef* USARTx, char *s);
void USART_puts_int8(USART_TypeDef* USARTx, uint8_t *s);
void USART_puts_int16(USART_TypeDef* USARTx, uint16_t *s);
void UART_DMA_Config();
uint16_t* getRecievedData();

#endif
