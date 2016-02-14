#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "clock.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>

void ADC_Config(void);
uint16_t ADC_Read(void);

// For ADC value
uint16_t adcValue = 0;
// For ADC value in string
char sAdcValue[5];

int main(void)
{
	// Set clock to 168MHz
	CLOCK_SetClockTo168MHz();
	// Initialize delay function
	DELAY_Init();
	// Initialize USART1 (PB6 Tx, PB7 Rx)
	USART_Config();

	// Initialize ADC
	ADC_Config();

	while (1)
	{
		// Read ADC value
		adcValue = ADC_Read();
		// Convert ADC value to string
		sprintf(sAdcValue, "%i\n", adcValue);
		// Send to PC via USART1
		USART_PutString(sAdcValue);

		DELAY_Ms(1000);
	}
}

void ADC_Config()
{
	// Enable clock for ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// Initialize GPIOB for ADC input
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Initialize ADC1
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);

	// Select input channel for ADC1
	// ADC1 channel 9 (PB1)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_84Cycles);
}

uint16_t ADC_Read()
{
	// Start ADC conversion
	ADC_SoftwareStartConv(ADC1);
	// Wait until ADC conversion finished
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	return ADC_GetConversionValue(ADC1);
}
