#include "imu.h"

char cmdData[4];
float IMUData[6] = {
0, 0, 0, 0, 0, 0
};
float *rxData;
bol accDataRequested = FALSE;
bol gyroDataRequested = FALSE;
uint8_t first = 0;

void requestGyroData()
{
	if (first == 1)
	{
		cmdData[0] = 0xf7; //START_PACKET;
		cmdData[1] = 0xdd; //GYRO_CMD;
		//cmdData[2]=0x04;
		cmdData[2] = 0x04;	
cmdData[3]=	0xe1;	//GYRO_CMD;
		USART_puts_char(UART4, &cmdData[0]);
		
	}
	if(first==1){
		cmdData[0] = 0xf7; //START_PACKET;
		cmdData[1] = 0xde; //GYRO_CMD;
		cmdData[2] = 0xde;
		//cmdData[2]=0x04;
						//GYRO_CMD;
		USART_puts_char(UART4, &cmdData[0]);
		first=1;
	}
	
	if (gyroDataRequested == FALSE && accDataRequested == FALSE)
	{
		cmdData[0] = 0xf9;				//START_PACKET;
		cmdData[1] = 0x43;				//GYRO_CMD;
		//cmdData[2]=0x04;
		cmdData[2] = 0x43;		//GYRO_CMD;
		USART_puts_char(UART4, &cmdData[0]);
		//gyroDataRequested = TRUE;
	}

}
void requestAccData()
{
	if (gyroDataRequested == FALSE && accDataRequested == FALSE)
	{
//		cmdData[0] = START_PACKET;
		//	cmdData[1] = ACC_CMD;
		//	cmdData[2] = ACC_CMD;
		USART_puts_char(UART4, &cmdData);
		accDataRequested = TRUE;
	}

}

void imuManager()
{
	requestGyroData();
//	rxData =  (float *)getRecievedData();
//	if (gyroDataRequested == TRUE && accDataRequested == FALSE)
//	{
//		*(IMUData+gyro_pitch) = *rxData;
//		*(IMUData+gyro_roll) = *(rxData + 1);
//		*(IMUData+gyro_yaw) = *(rxData + 2);
//		gyroDataRequested=FALSE;
//		requestAccData();
//	}
//	if (gyroDataRequested == FALSE && accDataRequested == TRUE)
//	{
//		*(IMUData+acc_x) = *rxData;
//		*(IMUData+acc_y) = *(rxData + 1);
//		*(IMUData+acc_z) = *(rxData + 2);
//		accDataRequested=FALSE;
//		requestGyroData();
//	}

}
