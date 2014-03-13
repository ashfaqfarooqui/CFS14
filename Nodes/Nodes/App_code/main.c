#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//******************************************************************************
char start[] = "Hello world";
uint16_t data;
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
	char *TransmitStatus;
	uint8_t TransmitMailBox;
	uint8_t NODE = 0x00;

	STM_EVAL_LEDInit(LED_BLUE);
	STM_EVAL_LEDInit(LED_GREEN);
	STM_EVAL_LEDInit(LED_ORANGE);
	STM_EVAL_LEDInit(LED_RED);
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); ///run this before running OS
	setNode();
	//init_ADC();
	//init_Timer();

	//init_uart(115200);
	init_CAN_Communication();
	CAN_ReceiverInit(RxMessage);
	CAN_configureFilter(0, CAN_FilterMode_IdMask, CAN_FilterScale_32bit, 0x0000,
			0x0000, 0x0000, 0x0000, 0, ENABLE);

init_driverInterface(0x01);
	xTaskCreate(vLedBlinkBlue, (const signed char* )"Led Blink Task Blue",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vLedBlinkRed, (const signed char* )"Led Blink Task Red",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vLedBlinkGreen, (const signed char* )"Led Blink Task Green",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vLedBlinkOrange, (const signed char* )"Led Blink Task Orange",
			STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL);
//	xTaskCreate(vCANReceiver, (const signed char* )"CAN", STACK_SIZE_MIN, NULL,
	//		tskIDLE_PRIORITY, NULL);

	vTaskStartScheduler();
	vTaskDelay(900 / portTICK_RATE_MS);

}
//******************************************************************************

//******************************************************************************
void vLedBlinkBlue(void *pvParameters) {

	uint16_t i;
	for (;;) {

		
		if(GPIO_ReadInputDataBit(INPUTPORT,FANCONTROL)==RESET){
	SwitchWarningLight(ON);
			STM_EVAL_LEDOn(LED_RED);
	}
		else {
			SwitchWarningLight(OFF);
		}

		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

void vLedBlinkRed(void *pvParameters) {
		CanTxMsg txmsg;
	uint8_t data[3];
	data[0]=0xff;
	data[1]=0x01;
	data[2]=0xee;

for(;;)
{
	uint16_t t=64;
	if(STM_EVAL_PBGetState(BUTTON_USER)==SET)
	{
		STM_EVAL_LEDToggle(LED_RED);	
		txmsg=CAN_createMessage(0x01,CAN_RTR_Data,CAN_Id_Standard,3,&data[0]);
		CAN_transmit_data(txmsg);
		while(t--){
			
			}
	}
	


		vTaskDelay(750 / portTICK_RATE_MS);
}

	
}

void vLedBlinkGreen(void *pvParameters) {
for(;;)
	{

		vTaskDelay(250 / portTICK_RATE_MS);
	}
}

void vSerialSender(void *pvParameters) {
	for (;;) {
vTaskDelay(900 / portTICK_RATE_MS);
	}
}
void vLedBlinkOrange(void *pvParameters) {
	for (;;) {
		vTaskDelay(900 / portTICK_RATE_MS);
	}
}
void vCANReceiver(void *pvParameters) {
	CanRxMsg RxMessage;
	CAN_ReceiverInit(&RxMessage);
	for (;;) {
		if ((CAN_MessagePending(CAN1, CAN_FIFO0) > 0)
				&& CAN_GetITStatus(CAN1, CAN_IT_FMP0)) {
			CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

		}
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

//******************************************************************************

void setNode() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(INPUTPORT, &GPIO_InitStructure);

	NODE = NODE | 1 << GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
			| 1 << GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
}
