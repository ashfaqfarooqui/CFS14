#include "main.h"
#include "tests.h"


//******************************************************************************
static __IO uint32_t uwLsiFreq = 0;

__IO uint32_t uwTimingDelay = 0;
__IO uint32_t uwCaptureNumber = 0;
__IO uint32_t uwPeriodValue = 0;
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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); ///run this before running OS
	initializeSystem();

	createTaskDAQ();
	xTaskCreate(vRecieveCan, (const signed char* )"Recieving Can",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vUpdateInputs,
			(const signed char* )"Update digital and analog inputs",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vUpdateADC,
			(const signed char* )"Update digital and analog inputs",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vPerformSwitchAction,
			(const signed char* )"perform switch action", STACK_SIZE_MIN, NULL,
			tskIDLE_PRIORITY, NULL);
	xTaskCreate(vIWDGUpdate, (const signed char* )"IWDG update", STACK_SIZE_MIN,
			NULL, tskIDLE_PRIORITY, NULL);
//	xTaskCreate(vUpdateWheelSpeedLeft,
//			(const signed char* )"Update wheel speed left", STACK_SIZE_MIN,
//			NULL, tskIDLE_PRIORITY, NULL);
//	xTaskCreate(vUpdateWheelSpeedRight,
//			(const signed char* )"Update wheel speed right", STACK_SIZE_MIN,
//			NULL, tskIDLE_PRIORITY, NULL);

	if (THIS_NODE == REAR_NODE)
	{
		xTaskCreate(vautoShiftMgr, (const signed char* )"auto shifting",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vGearShifting, (const signed char* )"Gear shifting",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
//		xTaskCreate(vIMUManager, (const signed char* )"IMU manager",
//				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vLaunchControl,
				(const signed char* )"launch control manager", STACK_SIZE_MIN,
				NULL, tskIDLE_PRIORITY, NULL);

		//DAQ
		xTaskCreate(vSendGear, (const signed char* )"send gear Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendWaterTemp, (const signed char* )"send oiltemp Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
//		xTaskCreate(vSendAcc, (const signed char* )"send acc Data",
//				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
//		xTaskCreate(vSendGyro, (const signed char* )"send gyro Data",
//				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendOilPressure,
				(const signed char* )"send oilpressure Data", STACK_SIZE_MIN,
				NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vSendGearDataRaw, (const signed char* )"send gear raw Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vNeutralMgr, (const signed char* )"neutral Data",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vsendCaliperTemp, (const signed char* )"send caliper temp Data",
					STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	}
	if (THIS_NODE == FRONT_NODE)
	{
		xTaskCreate(vSafetyCheck, (const signed char* )"safety", STACK_SIZE_MIN,
				NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vCoolingSystem, (const signed char* )"cooling system",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vCalculteBrakeBias, (const signed char* )"brakebias",
				STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
		xTaskCreate(vTelematics, (const signed char* )"telematics mgr",
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
	vTaskDelay(90 / portTICK_RATE_MS);

}
//******************************************************************************
void createTaskDAQ()
{
	xTaskCreate(vSendWheelSpeed, (const signed char* )"send wheelspeed Data",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vSendDamperTravel, (const signed char* )"send damper Data",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);

}

void initIWDG()
{

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);

	IWDG_SetReload(0x0fff);
	IWDG_Enable();

}
void initializeSystem()
{
	initIWDG();
	init_uart(115200);
	init_inputCapture();
	init_CAN_Communication();
	CAN_ReceiverInit(&RxMessage);
	CAN_configureFilter(0, CAN_FilterMode_IdMask, CAN_FilterScale_32bit, 0x0000,
			0x0000, 0x0000, 0x0000, 0, ENABLE);
	init_actuators();
	init_driverInterface();
	init_ADC();
	init_gearShiftTimer();
	init_pwm_config();
	if (THIS_NODE == FRONT_NODE)
	{
		setFanSpeed(PERIOD_FAN);
		setCoolantPumpSpeed(PERIOD_FAN);
	} else
	{
		
		initPWMCutIgnition();
		cutIgnition(PERIOD_CUT_IGNITION);
		ActuateShiftUp(0);
		ActuateShiftDown(0);
	}
	
}

//******************************************************************************

void vNeutralMgr(void *pvParameters)
{
	while (1)
	{
		uint8_t try=0;
		uint8_t gear = GetGearPosition();
		if ((neutralSwitch == TRUE && gear == 1) || ToNeutral == TRUE)
		{
			neutralMgr();
			try++;
			if(try>5)
			{
				ToNeutral=FALSE;
				neutralSwitch=FALSE;
				try=0;
			}

		}
		vTaskDelay(20 / portTICK_RATE_MS);
	}
}

void vsendCaliperTemp(void *pvParameters)
{
	while (1)
		{

			sendCaliperTemp();
			vTaskDelay(1500 / portTICK_RATE_MS);
		}
}
void vSendGearDataRaw(void *pvParameters)
{
	while (1)
	{

		sendGearRaw();
		vTaskDelay(1500 / portTICK_RATE_MS);
	}
}
void vLaunchControl(void *pvParameters)
{
	while (1)
	{

		launchControl();
		vTaskDelay(400 / portTICK_RATE_MS);
	}
}
void vIMUManager(void *pvParameters)
{
	while (1)
	{

		imuManager();

		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}
void vCoolingSystem(void *pvParameters)
{
	while (1)
	{
		coolingControl();
		vTaskDelay(4000 / portTICK_RATE_MS);
	}
}
void vautoShiftMgr(void *pvParameters)
{
	while (1)
	{
		autoShiftManager();
		vTaskDelay(50 / portTICK_RATE_MS);
	}
}
void vGearShifting(void *pvParameters)
{
	while (1)
	{

		gearShiftManager();
		sendSwitchState();
		vTaskDelay(30 / portTICK_RATE_MS);
	}
}
void vSafetyCheck(void *pvParameters)
{
	while (1)
	{
		safetyCheck();
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}
void vUpdateADC(void *pvParameters)
{
	while (1)
	{
		saveRawADCData();
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}
void vUpdateInputs(void *pvParameters)
{
	while (1)
	{
		updateSwitches();
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}
void vPerformSwitchAction(void *pvParameters)
{
	while (1)
	{

		switchAction();
		vTaskDelay(20 / portTICK_RATE_MS);
	}
}
void vRecieveCan(void *pvParameters)
{
	while (1)
	{
		readMessages();
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}
void vSendWheelSpeed(void *pvParameters)
{
	while (1)
	{

		sendWheelSpeed(); //
		vTaskDelay(50 / portTICK_RATE_MS);
	}
}
void vSendDamperTravel(void *pvParameters)
{
	while (1)
	{

		sendDamperPosition(); //
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
void vSendGear(void *pvParameters)
{
	while (1)
	{
		sendGear();
		vTaskDelay(1000 / portTICK_RATE_MS);
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

		vTaskDelay(500 / portTICK_RATE_MS);
	}
}
void vSendOilPressure(void *pvParameters)
{
	while (1)
	{
		sendOilPressure(); //
		vTaskDelay(250 / portTICK_RATE_MS);
	}
}
void vSendWaterTemp(void *pvParameters)
{
	while (1)
	{
		sendOilTemprature();
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}
void vSendAcc(void *pvParameters)
{
	while (1)
	{
//		sendAccData();
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}
void vSendGyro(void *pvParameters)
{
	while (1)
	{
//		sendGyroData();
		vTaskDelay(250 / portTICK_RATE_MS);
	}
}
void vSendBreakPressureData(void *pvParameters)
{
	while (1)
	{
		sendBrakePressure(); //

		vTaskDelay(20 / portTICK_RATE_MS);
	}
}
void vCalculteBrakeBias(void *pvparameters)
{
	while (1)
	{
		calculateBrakeBias();
		vTaskDelay(10000 / portTICK_RATE_MS);

	}
}

void vTelematics(void *pvParameters)
{
	while (1)
	{
		telematicsMgr();

		vTaskDelay(40 / portTICK_RATE_MS);
	}
}
void vIWDGUpdate(void *pvParameters)
{
	while (1)
	{
		IWDG_ReloadCounter();
		vTaskDelay(3000 / portTICK_RATE_MS);
	}
}

//******************************************************************************

