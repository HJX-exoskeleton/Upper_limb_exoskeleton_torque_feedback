#ifndef PID_H
#define PID_H

#include "user.h"

/******************************************************************************/
void PID_init(MOTORController *M);
float PIDoperator(PIDController* PID,float error);
/******************************************************************************/

#endif

