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

#if THIS_NODE==FRONT_NODE
//freq
#define W_SPEED_R 1
#define W_SPEED_L 2
//analog
#define DAMPER_TRAVEL_L 3
#define DAMPER_TRAVEL_R 4
#define DISC_TEMP 5
#define BRAKE_PRESSURE_FRONT 6
#define BRAKE_PRESSURE_REAR 7
//sensor
#define STEERING_ANGLE

#elif THIS_NODE==REAR_NODE
//freq
#define W_SPEED_R 1
#define W_SPEED_L 2
//analog
#define DAMPER_TRAVEL_L 3
#define DAMPER_TRAVEL_R 4
#define GEAR_POSITION 5
#define OIL_PRESSURE 6
#define OIL_TEMPRATURE 7
#define X_ACC 8
#define Y_ACC 9
#define ROLL_GYRO 10
#define PITCH_GYRO 11
#define YAW_GYRO 12

#endif




static unsigned int NODE = 0x00;


extern int rawAnalogState[NUMBER_OF_ANALOG_IN_PER_NODE];

extern unsigned int sensorData[256];

#endif
