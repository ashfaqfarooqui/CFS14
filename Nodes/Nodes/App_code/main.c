
#include "main.h"

//******************************************************************************
int main(void) {

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
	int i;
	CanRxMsg RxMessage;
	char *TransmitStatus;
	uint8_t TransmitMailBox;
	init_uart(9600);
	NVIC_Config();
	init_CAN_Communication();
	CAN_ReceiverInit();
	STM_EVAL_LEDInit(LED_BLUE);
	STM_EVAL_LEDInit(LED_GREEN);
	STM_EVAL_LEDInit(LED_ORANGE);
	STM_EVAL_LEDInit(LED_RED);


//	xTaskCreate(vLedBlinkBlue, (const signed char* )"Led Blink Task Blue",
//			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
//	xTaskCreate(vLedBlinkRed, (const signed char* )"Led Blink Task Red",
//			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
//	xTaskCreate(vLedBlinkGreen, (const signed char* )"Led Blink Task Green",
//			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vLedBlinkOrange, (const signed char* )"Led Blink Task Orange",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vCANReceiver, (const signed char* )"CAN",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);

	//vTaskStartScheduler();
do{
	CAN_ReceiverInit(&RxMessage);
	TransmitMailBox=transmit_data();
	TransmitMailBox=transmit_data();
}
	while(CAN_TransmitStatus(CAN1,TransmitMailBox)==CAN_TxStatus_Failed);
	
	for (;;) {
			if ((CAN_MessagePending(CAN1, CAN_FIFO0) > 0)) {
				CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
				receiverTest(RxMessage);

			}
			//vTaskDelay(900 / portTICK_RATE_MS);
		}

}
//******************************************************************************

//******************************************************************************
void vLedBlinkBlue(void *pvParameters) {
	for (;;) {
		STM_EVAL_LEDToggle(LED_BLUE);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

void vLedBlinkRed(void *pvParameters) {
	for (;;) {
		STM_EVAL_LEDToggle(LED_RED);
		vTaskDelay(750 / portTICK_RATE_MS);
	}
}

void vLedBlinkGreen(void *pvParameters) {
	for (;;) {
		STM_EVAL_LEDToggle(LED_GREEN);
		vTaskDelay(250 / portTICK_RATE_MS);
	}
}

void vSerialSender(void *pvParameters) {
	for (;;) {

	}
}
void vLedBlinkOrange(void *pvParameters) {
	for (;;) {
		STM_EVAL_LEDToggle(LED_ORANGE);
		vTaskDelay(900 / portTICK_RATE_MS);
	}
}
void vCANReceiver(void *pvParameters) {
	CanRxMsg RxMessage;
	CAN_ReceiverInit(&RxMessage);
	for (;;) {
				if ((CAN_MessagePending(CAN1, CAN_FIFO0) > 0)&&CAN_GetITStatus(CAN1,CAN_IT_FMP0)) {
					CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
					receiverTest(RxMessage);

				}
				vTaskDelay(500 / portTICK_RATE_MS);
			}
}

//******************************************************************************

