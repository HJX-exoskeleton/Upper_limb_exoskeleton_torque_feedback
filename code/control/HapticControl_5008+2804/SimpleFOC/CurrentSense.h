#ifndef CURRENTSENSE_H
#define CURRENTSENSE_H

#include "user.h" 

/******************************************************************************/
float getDCCurrent(MOTORController *M,float motor_electrical_angle);
DQCurrent_s getFOCCurrents(MOTORController *M,float angle_el);
/******************************************************************************/


#endif

