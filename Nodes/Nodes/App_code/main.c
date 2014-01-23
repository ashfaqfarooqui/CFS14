

//******************************************************************************
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "discoveryf4utils.h"
#include "stm32f4xx_usart.h"
#include "can.h"
//******************************************************************************

//******************************************************************************
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "croutine.h"
//******************************************************************************
void init_uart(uint32_t );


void vSerialSender(void *pvParameters);
void USART_puts(USART_TypeDef* USARTx, char *s);
void vLedBlinkBlue(void *pvParameters);
void vLedBlinkRed(void *pvParameters);
void vLedBlinkGreen(void *pvParameters);
void vLedBlinkOrange(void *pvParameters);
void vCANTask(void *pvParameters);
     GPIO_InitTypeDef GPIO_InitStruct;
		 NVIC_InitTypeDef NVIC_InitStructure1;
		 USART_InitTypeDef USART_InitStruct;
#define STACK_SIZE_MIN	128	/* usStackDepth	- the stack size DEFINED IN WORDS.*/

//******************************************************************************
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
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	init_uart(9600);
	init_CAN_Communication();
	
	STM_EVAL_LEDInit(LED_BLUE);
	STM_EVAL_LEDInit(LED_GREEN);
	STM_EVAL_LEDInit(LED_ORANGE);
	STM_EVAL_LEDInit(LED_RED);
	USART_puts(UART4,(char *) (transmit_data()));
	xTaskCreate( vLedBlinkBlue, (const signed char*)"Led Blink Task Blue", 
		STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( vLedBlinkRed, (const signed char*)"Led Blink Task Red", 
		STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( vLedBlinkGreen, (const signed char*)"Led Blink Task Green", 
		STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( vLedBlinkOrange, (const signed char*)"Led Blink Task Orange", 
		STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL );
//	xTaskCreate( vSerialSender, (const signed char*)"Serial Task", 
//		STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( vCANTask, (const signed char*)"CAN Task", 
		STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL );
	
	vTaskStartScheduler();
	
	
}
//******************************************************************************

//******************************************************************************
void vLedBlinkBlue(void *pvParameters)
{
	for(;;)
	{
		STM_EVAL_LEDToggle(LED_BLUE);
		vTaskDelay( 500 / portTICK_RATE_MS );
	}
}

void vLedBlinkRed(void *pvParameters)
{
	for(;;)
	{
		STM_EVAL_LEDToggle(LED_RED);
		vTaskDelay( 750 / portTICK_RATE_MS );
	}
}

void vLedBlinkGreen(void *pvParameters)
{
	for(;;)
	{
		STM_EVAL_LEDToggle(LED_GREEN);
		vTaskDelay( 250 / portTICK_RATE_MS );
	}
}

void vSerialSender(void *pvParameters)
{
	for(;;)
	{
		
		
	}
}
void vLedBlinkOrange(void *pvParameters)
{
	for(;;)
	{
//		USART_puts(UART4,"hello");
		vTaskDelay( 900/ portTICK_RATE_MS );
		STM_EVAL_LEDToggle(LED_ORANGE);
		vTaskDelay( 900 / portTICK_RATE_MS );
	}
}

void vCANTask(void *pvParameters)
{
	for(;;)
	{
		//USART_puts(UART4,(char *) (transmit_data()));
		vTaskDelay( 500 / portTICK_RATE_MS );
	}
}
void USART_puts(USART_TypeDef* USARTx, char *s){

        
	while(*s){
		while((USART_GetFlagStatus(UART4,USART_FLAG_TC)==SET))
		{
			USART_SendData(UART4,*s++);
			
		}
	}
	
}

void init_uart(uint32_t baudrate){

	USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_Init(GPIOC, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
GPIO_Init(GPIOC, &GPIO_InitStructure);

GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);


USART_InitStructure.USART_BaudRate=9600;
USART_InitStructure.USART_WordLength=USART_WordLength_8b;
USART_InitStructure.USART_StopBits=USART_StopBits_2;
USART_InitStructure.USART_Parity=USART_Parity_No;
USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
USART_Init(UART4,&USART_InitStructure);

USART_Cmd(UART4,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel =UART4_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

}
//******************************************************************************





