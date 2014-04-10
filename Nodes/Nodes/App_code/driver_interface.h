#ifndef __DRIVER_INTERFACE_H
#define __DRIVERINTERFACE_H_

#include "stm32f4xx.h"
#include "state.h"

#define FAST_SWITCH_DELAY 5 // 10ms
#define MEDIUM_SWITCH_DELAY 10 // 20ms
#define SLOW_SWITCH_DELAY 50 //100ms


#define GEARUP GPIO_Pin_7
#define GEARDOWN GPIO_Pin_8
#define LAUNCH_CONTROL GPIO_Pin_15
#define TRACTION_CONTROL GPIO_Pin_11
#define DATALOGGER GPIO_Pin_10
#define AUTOSHIFTING GPIO_Pin_12
#define FANCONTROL GPIO_Pin_13
#define E_CLUTCH GPIO_Pin_14


#define WARNINGLIGHT GPIO_Pin_14

#define INPUTPORT GPIOE
#define OUTPUTPORT GPIOC

//position in switch states for each switch
#define GEARUP_POS 0
#define GEARDOWN_POS 1
#define LC_POS 2
#define TC_POS 3
#define DL_POS 4
#define AS_POS 5
#define FC_POS 6
#define EC_POS 7



#define ON 0x21
#define OFF 0x22

void init_driverInterface();
void switchWarningLight(uint8_t );
void switchAction(void);
void setSwitchStates(uint8_t);
unsigned char debounceInput(int , int , int  );
void updateSwitches(void);

#endif

