#ifndef __SAFETY_H_
#define __SAFETY_H_

#include "can.h"
#include "driver_interface.h"

#define MAX 100
#define OIL 0x01
#define WATER 0x10

uint8_t shutDownEngine( );
void setFanSpeed( uint8_t);
#endif
