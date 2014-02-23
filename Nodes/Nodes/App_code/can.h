#ifndef __H_
#define __H_
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

char FAILED ='a';
char PASSED='z';

#define CANx                       CAN1
 #define CAN_CLK                    RCC_APB1Periph_CAN1
 #define CAN_RX_PIN                 GPIO_Pin_0
 #define CAN_TX_PIN                 GPIO_Pin_1
 #define CAN_GPIO_PORT              GPIOD
 #define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOD
 #define CAN_AF_PORT                GPIO_AF_CAN1
 #define CAN_RX_SOURCE              GPIO_PinSource0
 #define CAN_TX_SOURCE              GPIO_PinSource1
  
void transmit_data();
void init_CAN_Communication();
CanTxMsg CAN_createMessage(uint32_t StdId, uint8_t RTR, uint8_t IDE,  uint8_t DLC,uint8_t *data);
void CAN_configureFilter(uint8_t CAN_FilterNumber, uint8_t CAN_FilterMode,
		uint8_t CAN_FilterScale, uint16_t CANFilterIdHigh,
		uint16_t CAN_FilterIdLow, uint16_t CAN_FilterMaskIdHigh,
		uint16_t CAN_FilterMaskIdLow,uint16_t CAN_FilterFIFOAssignment,FunctionalState CAN_FilterActivation );
void CAN_ReceiverInit(CanRxMsg* );

char receiverTest(CanRxMsg );
void NVIC_Config(void);



#endif
