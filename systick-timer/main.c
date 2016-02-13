#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "clock.h"

__IO uint32_t msTick;

// This function will be called every 1ms
void SysTick_Handler(void)
{
	msTick--;
}

int main(void)
{
	// Set clock to 168MHz
	CLOCK_SetClockTo168MHz();

	// Update SystemCoreClock variable
	SystemCoreClockUpdate();
	// Make SysTick overflow every 1ms
	SysTick_Config(SystemCoreClock / 1000);

	// Enable clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Initialization for PD13 (orange LED) as output
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	while (1)
	{
		// Toggle LED
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

		// Set delay 1s (1000ms)
		msTick = 1000;
		// Wait until msTick is zero
		while (msTick);
	}
}
