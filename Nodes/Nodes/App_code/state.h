/** Add all the #defines for all sensors and variables to be used.
 * Also declare the variables to be used.
 */
#ifndef __STATE_H_
#define __STATE_H_

#define NUMBER_OF_NODES 2
#define NUMBER_OF_DIGITAL_IN_PER_NODE 10
#define NUMBER_OF_ANALOG_IN_PER_NODE 8

//define pins

#define FAN_OUT_PIN GPIO_Pin_6
#define FANOUT_PINSOURCE GPIO_PinSource6


#define FRONT_NODE 1
#define REAR_NODE 2
#define TEST_NODE 3
#define ALL_NODE 0


#define THIS_NODE FRONT_NODE
//#define THIS_NODE REAR_NODE

#define FRONT_NODE_OFFSET 0
#define REAR_NODE_OFFSET 8



//ANALOG SENSOR
#define AN_DAMPER_TRAVEL_FL 0
#define AN_DAMPER_TRAVEL_FR	1
#define AN_BRAKE_DISC_TEMP 2
#define AN_BRAKE_PRESSURE_F 3
#define AN_BRAKE_PRESSURE_R 4
#define AN_STEERING_ANGLE 5



#define AN_DAMPER_TRAVEL_RL 0
#define AN_DAMPER_TRAVEL_RR	1
#define AN_OIL_PRESSURE 2
#define AN_OIL_TEMPRATURE 3
#define AN_GEAR_POSITION 4
#define AN_CYLINDER_POSITION 5

//all sensors
#define DAMPER_TRAVEL_FL 1
#define DAMPER_TRAVEL_FR 2
#define DAMPER_TRAVEL_RL 3
#define DAMPER_TRAVEL_RR 4

#define OIL_PRESSURE 8
#define OIL_TEMPRATURE 9
#define ACC_X 10
#define ACC_Y 11
#define ACC_Z 12
#define GYRO_ROLL 13
#define GYRO_PITCH 14
#define GYRO_YAW 15
#define W_SPEED_FL 20
#define W_SPEED_FR 21
#define W_SPEED_RL 22
#define W_SPEED_RR 23

#define BRAKE_PRESSURE_F 25
#define BRAKE_PRESSURE_R 26
#define BRAKE_DISC_TEMP 27
#define STEERING_ANGLE 29
#define CYLINDER_POSITION 35
#define GEAR_POSITION 36
#define ENGINE_RPM 31

#define WHEEL_TEMP_FL1 80
#define WHEEL_TEMP_FL2 81
#define WHEEL_TEMP_FL3 82
#define WHEEL_TEMP_FR1 83
#define WHEEL_TEMP_FR2 84
#define WHEEL_TEMP_FR3 85
#define WHEEL_TEMP_RL1 86
#define WHEEL_TEMP_RL2 87
#define WHEEL_TEMP_RL3 88
#define WHEEL_TEMP_RR1 89
#define WHEEL_TEMP_RR2 90
#define WHEEL_TEMP_RR3 91


static unsigned int NODE = 0x00;


extern int rawAnalogState[NUMBER_OF_ANALOG_IN_PER_NODE];

extern unsigned int sensorData[150];

#endif
