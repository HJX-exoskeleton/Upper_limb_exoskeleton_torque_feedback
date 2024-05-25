

#ifndef MYPROJECT_H
#define MYPROJECT_H

/* Includes ------------------------------------------------------------------*/

#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "adc.h"
#include "iic.h"
#include "spi2.h"

#include "MagneticSensorI2C.h" 
#include "MagneticSensorSPI.h" 
#include "Sensor.h" 
#include "foc_utils.h"
#include "FOCMotor.h"
#include "BLDCmotor.h" 
#include "lowpass_filter.h" 
#include "pid.h"
#include "InlineCurrentSense.h"
#include "CurrentSense.h"

/******************************************************************************/
#define M1_Enable    GPIO_SetBits(GPIOB,GPIO_Pin_9)          //高电平使能
#define M1_Disable   GPIO_ResetBits(GPIOB,GPIO_Pin_9)        //低电平失能
#define M2_Enable    GPIO_SetBits(GPIOB,GPIO_Pin_3)          //高电平使能
#define M2_Disable   GPIO_ResetBits(GPIOB,GPIO_Pin_3)        //低电平失能
/******************************************************************************/
//M1 M2的SPI接口共用SPI2，只是CS引脚不同。SPI只能接同类型的编码器，不能混用
//I2C接口可以与SPI的混用 
//因为TLE5012B的TX和RX是连接在一起的，所以不能与AS5047P和MA730混用
//因为MA730的SPI接口为mode3，而AS5047P为mode1，所以不能混用 

#define M1_AS5600    0   //M1编码器类型只能选一
#define M1_AS5047P   1
#define M1_TLE5012B  0
#define M1_MA730     0

#define M2_AS5600    1   //M2编码器类型只能选一
#define M2_AS5047P   0
#define M2_TLE5012B  0
#define M2_MA730     0
/******************************************************************************/
#endif

