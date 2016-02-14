/**
  ******************************************************************************
  * @file	delay.h
  * @author	Yohanes Erwin Setiawan
  * @date	13 February 2016
  ******************************************************************************
  */

#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

void DELAY_Init(void);
void DELAY_Us(uint32_t us);
void DELAY_Ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
