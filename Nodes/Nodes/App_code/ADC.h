#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"
#include <stm32f4xx.h>

void init_ADC(void);
void DMA_Config(void);
void NVIC_Config_adc(void);

#define ADC_CDR_ADDRESS    ((uint32_t)0x4001204C)



#endif
