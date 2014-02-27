
#ifndef __STM32F4XX_USART_H_
#define __STM32F4XX_USART_H_
#include "stm32f4xx_usart.h"

void init_uart(uint32_t );
void USART_puts(USART_TypeDef* USARTx, char *s);


#endif