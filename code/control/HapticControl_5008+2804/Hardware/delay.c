
#include "delay.h"


/******************************************************************************/
//配置为1ms中断
void systick_CountInit(void)
{
	SysTick->LOAD = 72000;      //set reload register
  SysTick->VAL  = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; //时钟AHB不分频，中断使能，SysTick使能
}
/******************************************************************************/
uint32_t sysTickUptime;
void SysTick_Handler(void)  //systick中断入口函数
{
	sysTickUptime++;
}
/******************************************************************************/
//最多计时0xFFFFFFFF = 4294967295 us = 4294.9秒 = 71.58分，计满以后循环从0开始
uint32_t _micros(void)
{
	uint32_t  ms,cycle_cnt;
	
	do{
		ms = sysTickUptime;
		cycle_cnt = SysTick->VAL;
	} while (ms != sysTickUptime);
	
	return (ms * 1000) + (72000 - cycle_cnt) / 72;
}
/******************************************************************************/
//延时nus
void delay_us(unsigned long nus)
{
	uint32_t  t0,t1,diff;
	
	t0 = _micros();
	do{
		t1=_micros();
		if(t1>=t0)diff=t1-t0;
		else diff=0xffffffff-t0+t1;
	}while(diff < nus);
}
/******************************************************************************/
//延时nms
void delay_ms(unsigned long nms)
{
	uint32_t  t0,t1,diff;
	
	t0 = _micros();
	do{
		t1=_micros();
		if(t1>=t0)diff=t1-t0;
		else diff=0xffffffff-t0+t1;
	}while(diff < nms * 1000);
}
/******************************************************************************/
