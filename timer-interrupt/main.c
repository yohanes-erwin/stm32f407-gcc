#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "clock.h"
#include "delay.h"

void TIM_INT_Init(void);

void TIM2_IRQHandler()
{
	// Checks whether the TIM2 interrupt has occurred or not
	if (TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		// Toggle orange LED (PD13)
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

		// Clears the TIM2 interrupt pending bit
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

int main(void)
{
	// Set clock to 168MHz
	CLOCK_SetClockTo168MHz();
	// Initialize delay function
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

	// Initialize timer interrupt
	TIM_INT_Init();

	while (1)
	{
		// Toggle blue LED (GPIO15) every 2.5s
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		DELAY_Ms(2500);
	}
}

void TIM_INT_Init()
{
	// Enable clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// TIM2 initialization for overflow every 500ms
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1))
	// Update Event (Hz) = 84MHz / ((4199 + 1) * (9999 + 1)) = 0.5 Hz
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 4199;
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	// Enable TIM2 interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	// Start TIM2
	TIM_Cmd(TIM2, ENABLE);

	// Nested vectored interrupt settings
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
