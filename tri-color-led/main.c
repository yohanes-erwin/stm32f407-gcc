#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "clock.h"
#include "delay.h"

#define DELAY 500

void TIM_PWM_Init(void);
void setRed(uint16_t red);
void setGreen(uint16_t green);
void setBlue(uint16_t blue);

int main(void)
{
	// Set clock to 168MHz
	CLOCK_SetClockTo168MHz();
	// Initialize delay function
	DELAY_Init();

	// Initialize PWM
	TIM_PWM_Init();

	while (1)
	{
		// Red
		setRed(8399);
		setGreen(0);
		setBlue(0);
		DELAY_Ms(DELAY);

		// Orange
		setRed(8399);
		setGreen(4199);
		setBlue(0);
		DELAY_Ms(DELAY);

		// Yellow
		setRed(8399);
		setGreen(8399);
		setBlue(0);
		DELAY_Ms(DELAY);

		// Light green
		setRed(4199);
		setGreen(8399);
		setBlue(0);
		DELAY_Ms(DELAY);

		// Green
		setRed(0);
		setGreen(8399);
		setBlue(0);
		DELAY_Ms(DELAY);

		// Blue green
		setRed(0);
		setGreen(8399);
		setBlue(4199);
		DELAY_Ms(DELAY);

		// Light blue
		setRed(0);
		setGreen(8399);
		setBlue(8399);
		DELAY_Ms(DELAY);

		// Sky blue
		setRed(0);
		setGreen(4199);
		setBlue(8399);
		DELAY_Ms(DELAY);

		// Blue
		setRed(0);
		setGreen(0);
		setBlue(8399);
		DELAY_Ms(DELAY);

		// Purple
		setRed(4199);
		setGreen(0);
		setBlue(8399);
		DELAY_Ms(DELAY);

		// Pink
		setRed(8399);
		setGreen(0);
		setBlue(8399);
		DELAY_Ms(DELAY);

		// Light pink
		setRed(8399);
		setGreen(0);
		setBlue(4199);
		DELAY_Ms(DELAY);

		// White
		setRed(8399);
		setGreen(8399);
		setBlue(8399);
		DELAY_Ms(DELAY);
	}
}

void TIM_PWM_Init()
{
	// Enable clock for TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	// Initialize TIM4
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_Period = 8400 - 1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	// Start TIM4
	TIM_Cmd(TIM4, ENABLE);

	// Initialize PWM
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	// PWM channel for green LED
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	// PWM channel for red LED
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	// PWM channel for blue LED
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC4Init(TIM4, &TIM_OCInitStruct);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// Enable clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Set alternate function of PD12, PD14, and PD15 as PWM outputs
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	// Initialize PD12, PD14, and PD15 as outputs
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void setRed(uint16_t red)
{
	// Set red LED brightness
	TIM4->CCR3 = red;
}

void setGreen(uint16_t green)
{
	// Set green LED brightness
	TIM4->CCR1 = green;
}

void setBlue(uint16_t blue)
{
	// Set blue LED brightness
	TIM4->CCR4 = blue;
}
