#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "clock.h"
#include "delay.h"

void EXTI0_Init(void);

void EXTI0_IRQHandler(void)
{
	// Checks whether the interrupt is from EXTI0 or not
	if (EXTI_GetITStatus(EXTI_Line0))
	{
		// Toggle orange LED (PD13)
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

		// Clears the EXTI line pending bit
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

int main(void)
{
	// Set clock to 168MHz
	CLOCK_SetClockTo168MHz();
	// Delay initialization
	DELAY_Init();

	// Enable clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Initialize PD13 and PD15 as output for orange and blue LED
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	// External interrupt initialization
	EXTI0_Init();

	while (1)
	{
		// Toggle blue LED (PD15) every 5s
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		DELAY_Ms(5000);
	}
}

void EXTI0_Init()
{
	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// Enable clock for SYSCFG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// Initialize PA0 as input for user button
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Selects the PA0 as external interrupt source
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	// External interrupt settings
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);

	// Nested vectored interrupt settings (most important interrupt)
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
}
