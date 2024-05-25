
#include "MyProject.h"


/******************************************************************************/
float sensor_offset=0;   //设置零点偏移，通过串口设置，本次代码中没有使用
/******************************************************************************/
// shaft angle calculation
float shaftAngle(MOTORController *M)
{
  // if no sensor linked return previous value ( for open loop )
  //if(!sensor) return shaft_angle;
  return M->sensor_direction * getAngle(M) - sensor_offset;
}
// shaft velocity calculation
float shaftVelocity(MOTORController *M)
{
  // if no sensor linked return previous value ( for open loop )
  //if(!sensor) return shaft_velocity;
  return M->sensor_direction * LPFoperator(&M->LPF_vel,getVelocity(M));
}
/******************************************************************************/
float electricalAngle(MOTORController *M)
{
  return _normalizeAngle(M->shaft_angle * M->pole_pairs - M->zero_electric_angle);
}
/******************************************************************************/


