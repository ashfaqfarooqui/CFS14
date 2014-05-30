#include "main.h"
#include "tests.h"

//******************************************************************************

unsigned int sensorData[150] = {
0
};
int main(void)

{

	/*!< At this stage the microcontroller clock setting is already configured,
	 this is done through SystemInit() function which is called from startup
	 file (startup_stm32f4xx.s) before to branch to application main.
	 To reconfigure the default setting of SystemInit() function, refer to
	 system_stm32f4xx.c file
	 */

	/*!< Most systems default to the wanted configuration, with the noticeable 
	 exception of the STM32 driver library. If you are using an STM32 with
	 the STM32 driver library then ensure all the priority bits are assigned
	 to be preempt priority bits by calling
	 NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 ); before the RTOS is started.
	 */
	char *TransmitStatus;
	uint8_t TransmitMailBox;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); ///run this before running OS
	initializeSystem();

	createTaskDAQ();
	xTaskCreate(vRecieveCan, (const signed char* )"Recieving Can",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vUpdateInputs,
			(const signed char* )"Update digital and analog inputs",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vPerformSwitchAction,
			(const signed char* )"perform switch action", STACK_SIZE_MIN, NULL,
			tskIDLE_PRIORITY, NULL);
	xTaskCreate(vUpdateWheelSpeedLeft,
			(const signed char* )"Update wheel speed left", STACK_SIZE_MIN,
			NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vUpdateWheelSpeedRight,
			(const signed char* )"Update wheel speed right", STACK_SIZE_MIN,
			NULL, tskIDLE_PRIORITY, NULL);

	if (THIS_NODE == REAR_NODE)
	{
		xTaskCreate(vGearShifting, (const signed char* )"Gear shifting",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vIMUManager, (const signed char* )"IMU manager",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);

		//DAQ
		xTaskCreate(vSendGear, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendWaterTemp, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendAcc, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendGyro, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendOilPressure, (const signed char* )"send 10Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		
	}
	if (THIS_NODE == FRONT_NODE)
	{
		xTaskCreate(vSafetyCheck, (const signed char* )"safety", STACK_SIZE_MIN,
				NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vCoolingSystem, (const signed char* )"cooling system",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);

		//DAQ
		xTaskCreate(vSendBrakeDisc, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendSteeringAngle, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendBreakPressureData,
				(const signed char* )"send break pressure Data", STACK_SIZE_MIN,
				NULL, tskIDLE_PRIORITY, NULL);

	}

	vTaskStartScheduler();
	vTaskDelay(900 / portTICK_RATE_MS);

}
//******************************************************************************
void createTaskDAQ()
{
	xTaskCreate(vSendWheelSpeed, (const signed char* )"send 50Hz Data",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSendDamperTravel, (const signed char* )"send 50Hz Data",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);

}
void initializeSystem()
{

	init_uart(115200);
	init_inputCapture();
	init_actuators();
	init_CAN_Communication();
	CAN_ReceiverInit(&RxMessage);
	CAN_configureFilter(0, CAN_FilterMode_IdMask, CAN_FilterScale_32bit, 0x0000,
			0x0000, 0x0000, 0x0000, 0, ENABLE);

	init_driverInterface();
	init_ADC();
	if (THIS_NODE == FRONT_NODE)
	{
		init_pwm_config();
		setFanSpeed(100);
		setCoolantPumpSpeed(100);
	}
}
//******************************************************************************
void vIMUManager(void *pvParameters)
{
	while (1)
	{
		imuManager();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vCoolingSystem(void *pvParameters)
{
	while (1)
	{
		coolingControl();
		vTaskDelay(250 / portTICK_RATE_MS);
	}
}
void vGearShifting(void *pvParameters)
{
	while (1)
	{
		gearShiftManager();
		vTaskDelay(5 / portTICK_RATE_MS);
	}
}
void vSafetyCheck(void *pvParameters)
{
	while (1)
	{
		safetyCheck();
		vTaskDelay(5 / portTICK_RATE_MS);
	}
}
void vUpdateWheelSpeedLeft(void *pvParameters)
{
	while (1)
	{

		calculateWheelSpeedLeft();

		vTaskDelay(4 / portTICK_RATE_MS);

	}
}

void vUpdateWheelSpeedRight(void *pvParameters)
{
	while (1)
	{

		calculateWheelSpeedRight();

		vTaskDelay(4 / portTICK_RATE_MS);

	}
}
void vUpdateInputs(void *pvParameters)
{
	while (1)
	{
		updateSwitches();
		saveRawADCData();
		vTaskDelay(2 / portTICK_RATE_MS);
	}
}
void vPerformSwitchAction(void *pvParameters)
{
	while (1)
	{
		
		switchAction();
		vTaskDelay(25 / portTICK_RATE_MS);
	}
}
void vRecieveCan(void *pvParameters)
{
	while (1)
	{
		readMessages();
		vTaskDelay(1 / portTICK_RATE_MS);
	}
}
void vSendWheelSpeed(void *pvParameters)
{
	while (1)
	{
		
		sendWheelSpeed(); //
		vTaskDelay(20 / portTICK_RATE_MS);
	}
}
void vSendDamperTravel(void *pvParameters)
{
	while (1)
	{

		sendDamperPosition(); //
		vTaskDelay(20 / portTICK_RATE_MS);
	}
}
void vSendGear(void *pvParameters)
{
	while (1)
	{
		sendGear();
		vTaskDelay(50 / portTICK_RATE_MS);
	}
}
void vSendSteeringAngle(void *pvParameters)
{
	while (1)
	{
		sendSteeringAngle();
		vTaskDelay(50 / portTICK_RATE_MS);
	}
}
void vSendBrakeDisc(void *pvParameters)
{
	while (1)
	{
		sendBrakeDiscTemp();

		vTaskDelay(50 / portTICK_RATE_MS);
	}
}
void vSendOilPressure(void *pvParameters)
{
	while (1)
	{
		sendOilPressure(); //
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vSendWaterTemp(void *pvParameters)
{
	while (1)
	{
		sendOilTemprature();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vSendAcc(void *pvParameters)
{
	while (1)
	{
		sendAccData();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vSendGyro(void *pvParameters)
{
	while (1)
	{
		sendGyroData();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vSendBreakPressureData(void *pvParameters)
{
	while (1)
	{
		sendBrakePressure(); //

		vTaskDelay(200 / portTICK_RATE_MS);
	}
}
void vRequestRPMData(void *pvParameters)
{
	while (1)
	{
//			requestEngineRPM();//

//			requestTPS();
		vTaskDelay(200 / portTICK_RATE_MS);
	}
}

//******************************************************************************

