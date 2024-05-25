#ifndef SENSOR_LIB_H
#define SENSOR_LIB_H

#include "user.h"


typedef struct 
{
	unsigned short mag_cpol;
	unsigned short mag_cpha;
	unsigned short mag_BrtPre;
}MagnetSensor_Para;
/******************************************************************************/
void I2CandSPI_Init(void);
void MagneticSensor_Init(MOTORController *M);
float getAngle(MOTORController *M);
float getVelocity(MOTORController *M);
/******************************************************************************/

#endif
