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
	int i;
	for (i = 0; i < 149; i++)
	{
		sensorData[i] + 5;
		if (sensorData[i] > 1000)
		{
			sensorData[i] = 0;
		}
	}
	sensorData[STEERING_ANGLE]++;
	
	sensorData[W_SPEED_FL] += 2;
	sensorData[W_SPEED_FR] += 2;
	if (sensorData[W_SPEED_FL] > 100)
	{
		sensorData[W_SPEED_FL] = 10;
	}
	if (sensorData[W_SPEED_FR] > 200)
	{
		sensorData[W_SPEED_FR] = 10;
	}

	sensorData[DAMPER_TRAVEL_FL]++;
	sensorData[DAMPER_TRAVEL_FR]++;
	sensorData[DAMPER_TRAVEL_RL]++;
	sensorData[DAMPER_TRAVEL_RR]++;
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
	{
		sensorData[BRAKE_PRESSURE_F] = 1;

	}
	if (sensorData[BRAKE_PRESSURE_R] > 500)
	{
		sensorData[BRAKE_PRESSURE_R] = 1;

	}

}

void simulateEngineRpm()
{
	while (1)
	{
		CanTxMsg EngineRpm;
		uint8_t data[8];
		data[0] = 0;
		data[1] = 0;
		data[2] = (0x11) & 0xff;
		data[3] = (0x11 >> 8) & 0x0f;
		data[4] = 0;
		data[5] = 0;
		data[6] = 0;
		data[7] = 0;
		EngineRpm = CAN_createMessage_uint(0x280,
		CAN_RTR_Data, CAN_ID_STD, 8, &data[0]);
		while (CAN_Transmit(CAN1, &EngineRpm) != CAN_TxStatus_NoMailBox)
			;
	}
}
void testUART()
{
	char data[]="hello world";
	while(1)
	{	USART_puts_char(UART4,&data[0]);
		delay(100);
	}
}
