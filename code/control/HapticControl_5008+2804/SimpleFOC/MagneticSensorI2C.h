#ifndef MAGNETICSENSORAS5600_LIB_H
#define MAGNETICSENSORAS5600_LIB_H

#include "stm32f10x.h"

/******************************************************************************/
#define  AS5600_Address  0x36
#define  RAW_Angle_Hi    0x0C
//#define  RAW_Angle_Lo    0x0D
#define  AS5600_CPR      4096
/******************************************************************************/
unsigned short I2C_getRawCount(I2C_TypeDef* I2Cx);
/******************************************************************************/

#endif
