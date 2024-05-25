
#ifndef STM32_SPI2_H
#define STM32_SPI2_H

#include "stm32f10x.h"
#include "Sensor.h"

/******************************************************************************/
void SPI2_Init_(MagnetSensor_Para *p);
unsigned short SPIx_ReadWriteByte(unsigned short byte);
/******************************************************************************/


#endif
