#include "imu.h"



char cmdData[3];
float IMUData[6]={0,0,0,0,0,0};
float *rxData;
bol accDataRequested=FALSE;
bol gyroDataRequested=FALSE;

void requestGyroData()
{
	if (gyroDataRequested == FALSE && accDataRequested == FALSE)
	{
		cmdData[0] = START_PACKET;
		cmdData[1] = GYRO_CMD;
		cmdData[2] = GYRO_CMD;
		USART_puts_char(UART4, &cmdData);
		gyroDataRequested = TRUE;
	}

}
void requestAccData()
{
	if (gyroDataRequested == FALSE && accDataRequested == FALSE)
	{
		cmdData[0] = START_PACKET;
		cmdData[1] = ACC_CMD;
		cmdData[2] = ACC_CMD;
		USART_puts_char(UART4, &cmdData);
		accDataRequested = TRUE;
	}

}

void imuManager()
{
	rxData =  (float *)getRecievedData();
	if (gyroDataRequested == TRUE && accDataRequested == FALSE)
	{
		*(IMUData+gyro_pitch) = *rxData;
		*(IMUData+gyro_roll) = *(rxData + 1);
		*(IMUData+gyro_yaw) = *(rxData + 2);
		gyroDataRequested=FALSE;
		requestAccData();
	}
	if (gyroDataRequested == FALSE && accDataRequested == TRUE)
	{
		*(IMUData+acc_x) = *rxData;
		*(IMUData+acc_y) = *(rxData + 1);
		*(IMUData+acc_z) = *(rxData + 2);
		accDataRequested=FALSE;
		requestGyroData();
	}

}
