#include "tests.h"

void testCAN()
{
	while (1)
	{
		if (transreciever == transmitter)
		{
			uint16_t t;
			CanTxMsg txmsg;
			uint8_t data[3];
			data[0] = 0xff;
			data[1] = 0x01;
			data[2] = 0xee;

			t = 64;
			if (STM_EVAL_PBGetState(BUTTON_USER) == SET)
			{
				STM_EVAL_LEDToggle(LED_RED);
				txmsg = CAN_createMessage_uint(0x01, CAN_RTR_Data,
				CAN_Id_Standard, 3, &data[0]);
				CAN_transmit_data(txmsg);
				while (t--)
				{

				}
			}
		} else if (transreciever == reciever)
		{
			CanRxMsg* Rxmsg;
			Rxmsg = getRXmsg();

		}
	}

}
void testPWM(void)
{
	init_pwm_config();
	setFanSpeed(25);
}

void testDAQ()
{

//	sendWheelSpeed();
//	sensorData[W_SPEED_FL] += 2;
//	sensorData[W_SPEED_FR] += 2;
//	if (sensorData[W_SPEED_FL] > 100)
//	{
//		sensorData[W_SPEED_FL] = 10;
//	}
//	if (sensorData[W_SPEED_FR] > 200)
//	{
//		sensorData[W_SPEED_FR] = 10;
//	}


	sensorData[DAMPER_TRAVEL_FL]++;
	sensorData[DAMPER_TRAVEL_FR]++;
	sensorData[DAMPER_TRAVEL_RL]++;
	sensorData[DAMPER_TRAVEL_RR]++;
	sendDamperPosition();
	if (sensorData[DAMPER_TRAVEL_FL] > 100)
	{
		sensorData[DAMPER_TRAVEL_FL] = 1;
	}
	if (sensorData[DAMPER_TRAVEL_FR] > 100)
	{
		sensorData[DAMPER_TRAVEL_FR] = 2;
	}
	if (sensorData[DAMPER_TRAVEL_RL] > 100)
	{
		sensorData[DAMPER_TRAVEL_RL] = 3;
	}
	if (sensorData[DAMPER_TRAVEL_RR] > 100)
	{
		sensorData[DAMPER_TRAVEL_RR] = 4;
	}


	sensorData[BRAKE_PRESSURE_R]++;
	sensorData[BRAKE_PRESSURE_R]++;
	sendBrakePressure();
	if(sensorData[BRAKE_PRESSURE_F]>500)
	{
		sensorData[BRAKE_PRESSURE_F]=1;

	}
	if(sensorData[BRAKE_PRESSURE_R]>500)
	{
		sensorData[BRAKE_PRESSURE_R]=1;

	}

}
