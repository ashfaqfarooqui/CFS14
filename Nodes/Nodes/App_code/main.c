
#include "main.h"
#include "tests.h"

//******************************************************************************
uint16_t data;
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
	int i;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); ///run this before running OS
	init_ADC();
	//init_Timer();

	init_uart(115200);
//	init_Timer();
//	init_counter();
//	init_inputCapture();
	init_CAN_Communication();
	CAN_ReceiverInit(&RxMessage);
	CAN_configureFilter(0, CAN_FilterMode_IdMask, CAN_FilterScale_32bit, 0x0000,
			0x0000, 0x0000, 0x0000, 0, ENABLE);
testDAQ();

	init_driverInterface();
	testUART();


	createTaskDAQ();
	xTaskCreate(vRecieveCan, (const signed char* )"Recieving Can",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vUpdateSensors, (const signed char* )"Update Sensors",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	

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
	xTaskCreate(vSendBrakeDisc, (const signed char* )"send 40Hz Data",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSendSteeringAngle, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSendGear, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSendOilPressure, (const signed char* )"send 10Hz Data",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSendWaterTemp, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSendAcc, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSendGyro, (const signed char* )"send 40Hz Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSend5HzData, (const signed char* )"send 5Hz Data",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);

}
//******************************************************************************

void vUpdateSensors(void *pvParameters)
{
	while (1)
	{
		testDAQ();
		updateSwitches();
		//saveRawADCData();

		vTaskDelay(20 / portTICK_RATE_MS);
	}
}
void vRecieveCan(void *pvParameters)
{
	while (1)
	{
		readMessages();
		vTaskDelay(20 / portTICK_RATE_MS);
	}
}
void vSendWheelSpeed(void *pvParameters)
{
	while (1)
	{
		
		sendWheelSpeed();//
		vTaskDelay(20 / portTICK_RATE_MS);
	}
}
void vSendDamperTravel(void *pvParameters)
{
	while (1)
	{

		sendDamperPosition();//
		vTaskDelay(20 / portTICK_RATE_MS);
	}
}
void vSendGear(void *pvParameters)
{
	while (1)
	{
		sendGear();
		vTaskDelay(25 / portTICK_RATE_MS);
	}
}
void vSendSteeringAngle(void *pvParameters)
{
	while (1)
	{
			sendSteeringAngle();
		vTaskDelay(25 / portTICK_RATE_MS);
	}
}void vSendBrakeDisc(void *pvParameters)
{
	while (1)
	{
			sendBrakeDiscTemp();
			
		vTaskDelay(25 / portTICK_RATE_MS);
	}
}
void vSendOilPressure(void *pvParameters)
{
	while (1)
	{
		sendOilPressure();//
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
void vSend5HzData(void *pvParameters)
{
	while (1)
	{
		sendBrakePressure();//

		if (TESTMODE)
		{

			//TODO firewall temp wheel temp
		}
		vTaskDelay(200 / portTICK_RATE_MS);
	}
}

//******************************************************************************

