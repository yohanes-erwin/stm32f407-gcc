#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "clock.h"
#include "delay.h"

void TIM_Init(void);
void PWM_Init(void);

int main(void)
{
	// Set clock to 168MHz
	CLOCK_SetClockTo168MHz();
	// Initialize delay function
	DELAY_Init();

	// Initialize TIM4
	TIM_Init();
	// Initialize PWM
	PWM_Init();

	while (1)
	{
		// Set green LED brightness from 0 to max
		int i;
		for (i = 0; i <= 8399; i += 3)
		{
			TIM4->CCR1 = i;
			DELAY_Ms(1);
		}
	}
}

void TIM_Init()
{
	// Enable clock for TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	// Initialize timer
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	// Create 1kHz PWM
	// TIM4 is connected to APB1 bus that have default clock 84MHz
	// So, the frequency of TIM4 is 84MHz
	// We use prescaler 10 here
	// So, the frequency of TIM4 now is 8.4MHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 10;
	// TIM_Period determine the PWM frequency by this equation:
	// PWM_frequency = timer_clock / (TIM_Period + 1)
	// If we want 1kHz PWM we can calculate:
	// TIM_Period = (timer_clock / PWM_frequency) - 1
	// TIM_Period = (8.4MHz / 1kHz) - 1 = 8399
	TIM_TimeBaseInitStruct.TIM_Period = 8399;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	// Start TIM4
	TIM_Cmd(TIM4, ENABLE);
}

void PWM_Init()
{
	// Initialize PWM
	TIM_OCInitTypeDef TIM_OCInitStruct;
	// Common PWM settings
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	// Duty cycle calculation equation:
	// TIM_Pulse = (((TIM_Period + 1) * duty_cycle) / 100) - 1
	// Ex. 25% duty cycle:
	// 		TIM_Pulse = (((8399 + 1) * 25) / 100) - 1 = 2099
	//		TIM_Pulse = (((8399 + 1) * 75) / 100) - 1 = 6299
	// We initialize PWM value with duty cycle of 25%
	TIM_OCInitStruct.TIM_Pulse = 2099;
	TIM_OC1Init(TIM4, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// Set clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Set alternate function of PD12 as PWM output
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	// Green LED (PD12) is connected to TIM4_CH1 GPIO AF mapping
	// So, set PD12 as output
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}
