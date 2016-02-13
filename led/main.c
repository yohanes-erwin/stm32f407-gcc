#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void Delay(__IO uint32_t nCount);

int main(void)
{
	// Enable clock for GPIOD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // Initialize PD13 as output for LED
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    while (1)
    {
    	// Toggle LED
        GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        Delay(0x0FFFFF);
    }
}

// Delay function
void Delay(__IO uint32_t nCount)
{
    while (nCount--);
}
