#include "can.h"
#include "driver_interface.h"
#define MAX 100
#define OIL 0x01
#define WATER 0x10
uint8_t getTemprature( uint8_t);
void fanSpeed( uint8_t);

//TODO put tempratures into variables!
uint8_t getTemprature(uint8_t param) {
	if (param == OIL) {
		//send can rtr to get oil temp from ECU
	}
	if (param == WATER) {
		//send can rtr to get water temp from ECU
	}

}

void waterTempCheck() {
	uint8_t temp = getTemprature(OIL);
	if (temp > 120) {
		//TODO: shutdown engine
	} else if (temp > 110) {
		fanSpeed(MAX);
		SwitchWarningLight(ON);
	} else if (temp > 95) {
		fanSpeed(90);
	} else {
		fanSpeed(75);
	}
}

void oilTempCheck() {

	uint8_t temp = getTemprature(WATER);
	if (temp > 120) {
		//TODO: shutdown engine
	}

}
