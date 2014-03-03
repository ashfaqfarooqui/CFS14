#include "safety.h"

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
		setFanSpeed(MAX);
		SwitchWarningLight(ON);
	} else if (temp > 95) {
		setFanSpeed(90);
	} else {
		setFanSpeed(75);
	}
}

void oilTempCheck() {

	uint8_t temp = getTemprature(WATER);
	if (temp > 120) {
		//TODO: shutdown engine
	}

}

void setFanSpeed(uint8_t pwmSpeed)
{

}
