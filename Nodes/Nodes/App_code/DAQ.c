/** this code will have the functions to write the data onto the data logger.
 *
 */

#include "DAQ.h"

void sendWheelSpeed()
{
	CanTxMsg wheelSpeedMsg;
	uint8_t transmitStatus;
	uint8_t data[5];
	data[0] = CAN_NODE_FILTER;
	if (THIS_NODE == FRONT_NODE)
	{
		data[2] = (sensorData[W_SPEED_FL]) & 0xff;
		data[1] = ((sensorData[W_SPEED_FL] >> 8) & 0xff);
		data[4] = (sensorData[W_SPEED_FR]) & 0xff;
		data[3] = ((sensorData[W_SPEED_FR] >> 8) & 0xff);
	} else if (THIS_NODE == REAR_NODE)
	{
		data[2] = (sensorData[W_SPEED_RL]) & 0xff;
		data[1] = ((sensorData[W_SPEED_RL] >> 8) & 0xff);
		data[4] = (sensorData[W_SPEED_RR]) & 0xff;
		data[3] = ((sensorData[W_SPEED_RR] >> 8) & 0xff);
	}
	wheelSpeedMsg = CAN_createMessage_uint(CAN_ADR_WHEEL_SPEED, CAN_RTR_Data,
	CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &wheelSpeedMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);
}
void sendDamperPosition()
{
	CanTxMsg DamperPosMsg;
	uint8_t transmitStatus;
	uint8_t data[5];
	data[0] = CAN_NODE_FILTER;

	if (THIS_NODE == FRONT_NODE)
	{
		data[2] = (sensorData[DAMPER_TRAVEL_FL]) & 0xff;
		data[1] = ((sensorData[DAMPER_TRAVEL_FL] >> 8) & 0x0f);
		data[4] = (sensorData[DAMPER_TRAVEL_FR]) & 0xff;
		data[3] = ((sensorData[DAMPER_TRAVEL_FR] >> 8) & 0x0f);
	} else if (THIS_NODE == REAR_NODE)
	{
		data[2] = (sensorData[DAMPER_TRAVEL_RL]) & 0xff;
		data[1] = ((sensorData[DAMPER_TRAVEL_RL] >> 8) & 0x0f);
		data[4] = (sensorData[DAMPER_TRAVEL_RR]) & 0xff;
		data[3] = ((sensorData[DAMPER_TRAVEL_RR] >> 8) & 0x0f);
	}
	DamperPosMsg = CAN_createMessage_uint(CAN_ADR_DAMPER_POSITION, CAN_RTR_Data,
	CAN_ID_STD, 5, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &DamperPosMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);
}

void sendBrakePressure()
{
	CanTxMsg BrakePressureMsg;
	uint8_t transmitStatus;
	uint8_t data[5];
	data[0] = 0;
	data[2] = (sensorData[BRAKE_PRESSURE_F]) & 0xff;
	data[1] = (sensorData[BRAKE_PRESSURE_F] >> 8) & 0x0f;
	data[4] = (sensorData[BRAKE_PRESSURE_R]) & 0xff;
	data[3] = (sensorData[BRAKE_PRESSURE_R] >> 8) & 0x0f;
	BrakePressureMsg = CAN_createMessage_uint(CAN_ADR_BRAKE_PRESSURE,
	CAN_RTR_Data, CAN_ID_STD, 5, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &BrakePressureMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);

}
void sendOilPressure()
{
	CanTxMsg OilPressureMsg;
	uint8_t transmitStatus;
	uint8_t data[3];
	data[0] = 0;
	data[2] = (sensorData[OIL_PRESSURE]) & 0xff;
	data[1] = (sensorData[OIL_PRESSURE] >> 8) & 0x0f;

	OilPressureMsg = CAN_createMessage_uint(CAN_ADR_OIL_PRESSURE,
	CAN_RTR_Data, CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &OilPressureMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);

}
void sendOilTemprature()
{
	CanTxMsg OilTempratureMsg;
	uint8_t transmitStatus;
	uint8_t data[3];
	data[0] = 0;
	data[2] = (sensorData[OIL_TEMPRATURE]) & 0xff;
	data[1] = (sensorData[OIL_TEMPRATURE] >> 8) & 0x0f;
	OilTempratureMsg = CAN_createMessage_uint(CAN_ADR_OIL_TEMPRATURE,
	CAN_RTR_Data, CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &OilTempratureMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);

}
void sendGear()
{
	CanTxMsg GearMsg;
	uint8_t transmitStatus;
	uint8_t data[3];
	uint8_t gear;
	gear = GetGearPosition();
	data[0] = 0;
	data[2] = (gear) & 0x0f;
	data[1] = (gear >> 8) & 0x00;
	GearMsg = CAN_createMessage_uint(CAN_ADR_SHIFTING,
	CAN_RTR_Data, CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &GearMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);

}
void sendGearRaw()
{
	CanTxMsg GearMsg;
	uint8_t transmitStatus;
	uint8_t data[3];
	data[0] = 0;
	data[2] = (sensorData[GEAR_POSITION]) & 0xff;
	data[1] = (sensorData[GEAR_POSITION] >> 8) & 0x0f;
	GearMsg = CAN_createMessage_uint(CAN_ADR_SHIFTING_RAW,
	CAN_RTR_Data, CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &GearMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);

}
void sendSteeringAngle()
{

	CanTxMsg SteeringMsg;
	uint8_t transmitStatus;
	uint8_t data[3];
	data[0] = 0;
	data[1] = (sensorData[STEERING_ANGLE]) & 0xff;
	data[2] = (sensorData[STEERING_ANGLE] >> 8) & 0x0f;
	SteeringMsg = CAN_createMessage_uint(CAN_ADR_STEERING_WHEEL_ANGLE,
	CAN_RTR_Data, CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &SteeringMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);

}
void sendAccData()
{
	CanTxMsg AccMsg;
	uint8_t transmitStatus;
	uint8_t data[7];
	data[0] = 0;
	data[1] = (sensorData[ACC_X]) & 0xff;
	data[2] = (sensorData[ACC_X] >> 8) & 0x0f;
	data[3] = (sensorData[ACC_Y]) & 0xff;
	data[4] = (sensorData[ACC_Y] >> 8) & 0x0f;
	data[5] = (sensorData[ACC_Z]) & 0xff;
	data[6] = (sensorData[ACC_Z] >> 8) & 0x0f;
	AccMsg = CAN_createMessage_uint(CAN_ADR_ACC,
	CAN_RTR_Data, CAN_ID_STD, 7, &data[0]);

	do
	{
		transmitStatus = CAN_Transmit(CAN1, &AccMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);
}

void sendGyroData()
{
	CanTxMsg GyroMsg;
	uint8_t transmitStatus;
	uint8_t data[7];
	data[0] = 0;
	data[2] = (sensorData[GYRO_ROLL]) & 0xff;
	data[1] = (sensorData[GYRO_ROLL] >> 8) & 0x0f;
	data[4] = (sensorData[GYRO_YAW]) & 0xff;
	data[3] = (sensorData[GYRO_YAW] >> 8) & 0x0f;
	data[6] = (sensorData[GYRO_PITCH]) & 0xff;
	data[5] = (sensorData[GYRO_PITCH] >> 8) & 0x0f;
	GyroMsg = CAN_createMessage_uint(CAN_ADR_GYRO,
	CAN_RTR_Data, CAN_ID_STD, 7, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &GyroMsg);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);
}

void sendBrakeDiscTemp()
{
	CanTxMsg BrakeDisc;
	uint8_t transmitStatus;
	uint8_t data[7];
	data[0] = 0;
	data[2] = (sensorData[BRAKE_DISC_TEMP]) & 0xff;
	data[1] = (sensorData[BRAKE_DISC_TEMP] >> 8) & 0x0f;

	BrakeDisc = CAN_createMessage_uint(CAN_ADR_BRAKE_DISC_TEMP,
	CAN_RTR_Data, CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &BrakeDisc);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);
}
void sendGearTime(uint8_t shiftDirection)
{
	CanTxMsg GearTime;
	uint8_t transmitStatus;
	uint8_t data[3];
	uint16_t time=0;
 time = getTimerValue();
	data[0] = shiftDirection;
	data[2] = (time) & 0xff;
	data[1] = (time >> 8) & 0xff;

	GearTime = CAN_createMessage_uint(CAN_ADR_GEAR,
	CAN_RTR_Data, CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &GearTime);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);
}
void sendSwitchState()
{
	CanTxMsg switchStates;
	uint8_t transmitStatus;
	uint8_t data[7];
	data[0] = 0;
	data[2] = (sensorData[SWITCHSTATE]) & 0xff;
	data[1] = (sensorData[SWITCHSTATE] >> 8) & 0xff;

	switchStates = CAN_createMessage_uint(CAN_ADR_SWITCHSTATE,
	CAN_RTR_Data, CAN_ID_STD, 3, &data[0]);
	do
	{
		transmitStatus = CAN_Transmit(CAN1, &switchStates);
	} while (transmitStatus == CAN_TxStatus_NoMailBox);
	sensorData[SWITCHSTATE] = 0;
}

