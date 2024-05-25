
#ifndef STM32_DELAY_H
#define STM32_DELAY_H
/******************************************************************************/

#include "stm32f10x.h"

void systick_CountInit(void);
uint32_t _micros(void);
void delay_us(unsigned long nus);
void delay_ms(unsigned long nms);
/******************************************************************************/

#endif

