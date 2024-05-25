#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

#include "user.h"

/******************************************************************************/
void LPF_init(MOTORController *M);
float LPFoperator(LowPassFilter* LPF,float x);
/******************************************************************************/

#endif

