
#ifndef __TESTS_H_
#define __TESTS_H_

#include "can.h"
#include "timer.h"
#include "safety.h"
#include "DAQ.h"
#include "state.h"

#define transmitter 0x01
#define reciever 0x10

#define transreciever transmitter

void testCAN(void);
void testPWM(void);
void testDAQ(void);
void simulateEngineRpm(void);
void testUART();
#endif
