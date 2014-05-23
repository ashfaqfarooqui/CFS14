/*
 * megasquirt.c
 *
 *  Created on: May 16, 2014
 *      Author: Zeyang Geng
 */
#include "megasquirt.h"

/********** GLOBAL DEFINATION **********/

/***************************************/
 unsigned short data[3]={0,0,0};
unsigned short transmitStatus=0;
void requestEngineRPM()
{
	
	CanTxMsg TxMessage;
	TxMessage.IDE = CAN_Id_Extended;
	TxMessage.RTR = CAN_RTR_REMOTE;
	TxMessage.ExtId = 0xDA02D00;
	TxMessage.DLC = 3;
	data[0] = 0x08;
	data[1] = 0x00;
	data[2] = 0xE0;
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &TxMessage);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);

}
void requestTPS()
{

	CanTxMsg TxMessage;
	TxMessage.IDE = CAN_Id_Extended;
	TxMessage.RTR = CAN_RTR_REMOTE;
	TxMessage.ExtId = 0xDA02103;
	TxMessage.DLC = 3;
	data[0] = 0x08;
	data[1] = 0x00;
	data[2] = 0xE0;
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &TxMessage);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);

}
