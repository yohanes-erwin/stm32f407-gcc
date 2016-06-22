/**
  ******************************************************************************
  * Configuration:
  * 1. Compile Settings: FPU soft, FreeRTOS include paths, HSE_VALUE=8000000
  *
  * 2. FreeRTOSConfig.h:
  * 	Change configUSE_IDLE_HOOK, configUSE_TICK_HOOK,
  * 	configCHECK_FOR_STACK_OVERFLOW, and configUSE_MALLOC_FAILED_HOOK to "0"
  * 	(FreeRTOSConfig.h).
  *
  * 3. FreeRTOSConfig.h:
  * 	Change configCPU_CLOCK_HZ to "((unsigned long) 168000000)"
  *
  * 4. FreeRTOSConfig.h:
  * 	Add this definitions for map FreeRTOS port interrupt handlers to their CMSIS
  *		standard names:
  * 		#define vPortSVCHandler SVC_Handler
  *			#define xPortPendSVHandler PendSV_Handler
  *			#define xPortSysTickHandler SysTick_Handler
  *
  * 5. Add files to project
  * 	- FreeRTOS\croutine.c
  * 	- FreeRTOS\event_groups.c
  * 	- FreeRTOS\list.c
  * 	- FreeRTOS\queue.c
  * 	- FreeRTOS\tasks.c
  * 	- FreeRTOS\timers.c
  * 	- FreeRTOS\portable\GCC\ARM_CM4F\port.c
  * 	- FreeRTOS\portable\MemMang\heap_1.c
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "clock.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/task.h"

void ledInit(void);
void vTaskLedRed(void *p);
void vTaskLedOrange(void *p);
void vTaskLedGreen(void *p);
void vTaskLedBlue(void *p);

int main(void)
{
	CLOCK_SetClockTo168MHz();
	SystemCoreClockUpdate();

	ledInit();

	// Create task
	xTaskCreate(vTaskLedRed, (const char*) "LED Red", 128, NULL, 1, NULL);
	xTaskCreate(vTaskLedOrange, (const char*) "LED Orange", 128, NULL, 1, NULL);
	xTaskCreate(vTaskLedGreen, (const char*) "LED Green", 128, NULL, 1, NULL);
	xTaskCreate(vTaskLedBlue, (const char*) "LED Blue", 128, NULL, 1, NULL);

	// Start RTOS
	vTaskStartScheduler();

	return 0;
}

void ledInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Initialize PD12, PD13, PD14, PD15 as output
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void vTaskLedRed(void *p)
{
	for (;;)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		vTaskDelay(100/portTICK_RATE_MS);
	}
}

void vTaskLedOrange(void *p)
{
	for (;;)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		vTaskDelay(250/portTICK_RATE_MS);
	}
}

void vTaskLedGreen(void *p)
{
	for (;;)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		vTaskDelay(500/portTICK_RATE_MS);
	}
}

void vTaskLedBlue(void *p)
{
	for (;;)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}
