/**
  ******************************************************************************
  * @file	usart.h
  * @author	Yohanes Erwin Setiawan
  * @date	13 February 2016
  ******************************************************************************
  */

#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

void USART_Config(void);
void USART_PutChar(char c);
void USART_PutString(char *s);
uint16_t USART_GetChar(void);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
