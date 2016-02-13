#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "clock.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
	// Set clock to 168MHz
	CLOCK_SetClockTo168MHz();
	// Initialize delay function
	DELAY_Init();

	// Enable clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Initialization of PD13 (for orange LED)
	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitDef);

	// Call USART1 configuration
	USART_Config();

	// Send "Hello, World!" to PC
	USART_PutString("Hello, World!\n");

	while (1)
	{
		// Get a char from PC
		uint16_t data = USART_GetChar();

		if (data == 'H')
		{
			// If received char is 'H' then turn on orange LED
			GPIO_SetBits(GPIOD, GPIO_Pin_13);
		}
		else if (data == 'L')
		{
			// If received char is 'L' then turn off orange LED
			GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		}
	}
}
