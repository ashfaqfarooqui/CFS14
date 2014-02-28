#ifndef __H
#define __H_

#include "stm32f4xx.h"


#define GEARUP GPIO_Pin_7
#define GEARDOWN GPIO_Pin_8
#define INPUTPORT GPIOE
#define OUTPUTPORT GPIOC
#define WARNINGLIGHT GPIO_Pin_14

//position in switch states for each switch
#define GEARUPPOS 0
#define GEARDOWNPOS 1
#define LC 2
#define TC 3

#define FRONT_NODE 0x01
#define REAR_NODE 0x02
#define ON 0x21
#define OFF 0x22

void init_driverInterface(uint8_t node);
void SwitchWarningLight(uint8_t ONOFF);


#endif
