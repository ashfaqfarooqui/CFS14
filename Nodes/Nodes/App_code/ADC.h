#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"
#include <stm32f4xx.h>
#include "state.h"


void init_ADC(void);
void DMA_Config(void);
void NVIC_Config_adc(void);
void saveRawADCData(void);
float convertData(uint8_t);
int processAdcSamples(uint8_t );
#define ADC_DR_ADDRESS    ((uint32_t)0x4001204C)

#endif
