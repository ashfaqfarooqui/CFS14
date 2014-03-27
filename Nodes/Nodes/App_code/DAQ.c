/** this code will have the functions to write the data onto the data logger.
 *
 */

#include "DAQ.h"

void sendWheelSpeed()
{
	CanTxMsg wheelSpeedMsg;
	char data[3];
	data[0]=CAN_NODE_FILTER;
	data[1]=(sensorData[W_SPEED_L]>>8)&0xff;
	data[2]=(sensorData[W_SPEED_R]>>8)&0xff;

	wheelSpeedMsg=CAN_createMessage_int(CAN_ADR_WHEEL_SPEED,CAN_RTR_Data,CAN_ID_STD,3,&data[0]);
	while(CAN_Transmit(CAN1,wheelSpeedMsg)!=CAN_TxStatus_NoMailBox);
}
void sendDamperPosition()
{
	CanTxMsg DamperPosMsg;
	char data[5];
	data[0]=CAN_NODE_FILTER;
	data[1]=(sensorData[DAMPER_TRAVEL_L])&0xff;
	data[2]=(sensorData[DAMPER_TRAVEL_L]>>8)&0x0f;
	data[3]=(sensorData[DAMPER_TRAVEL_R])&0xff;
	data[4]=(sensorData[DAMPER_TRAVEL_R]>>8)&0x0f;

	DamperPosMsg=CAN_createMessage_int(CAN_ADR_DAMPER_POSITION,CAN_RTR_Data,CAN_ID_STD,5,&data[0]);
	while(CAN_Transmit(CAN1,DamperPosMsg)!=CAN_TxStatus_NoMailBox);
}

void sendBrakePressure()
{
	CanTxMsg BrakePressureMsg;
	char data[5];
	data[0]=0;
	data[1]=(sensorData[BRAKE_PRESSURE_FRONT])&0xff;
	data[2]=(sensorData[BRAKE_PRESSURE_FRONT]>>8)&0x0f;
	data[3]=(sensorData[BRAKE_PRESSURE_REAR])&0xff;
	data[4]=(sensorData[BRAKE_PRESSURE_REAR]>>8)&0x0f;
	BrakePressureMsg=CAN_createMessage_int(CAN_ADR_BRAKE_PRESSURET,CAN_RTR_Data,CAN_ID_STD,5,&data[0]);
	while(CAN_Transmit(CAN1,BrakePressureMsg)!=CAN_TxStatus_NoMailBox);


}
void sendOilPressure()
{
	CanTxMsg OilPressureMsg;
	char data[5];
	data[0]=0;
	data[1]=(sensorData[OI])&0xff;
	data[2]=(sensorData[BRAKE_PRESSURE_FRONT]>>8)&0x0f;
	data[3]=(sensorData[BRAKE_PRESSURE_REAR])&0xff;
	data[4]=(sensorData[BRAKE_PRESSURE_REAR]>>8)&0x0f;
	BrakePressureMsg=CAN_createMessage_int(CAN_ADR_BRAKE_PRESSURET,CAN_RTR_Data,CAN_ID_STD,5,&data[0]);
	while(CAN_Transmit(CAN1,BrakePressureMsg)!=CAN_TxStatus_NoMailBox);


}
