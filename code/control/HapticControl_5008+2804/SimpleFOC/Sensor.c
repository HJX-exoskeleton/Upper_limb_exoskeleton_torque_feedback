

#include "MyProject.h"


MagnetSensor_Para  AS5047P_para,TLE5012B_para,MA730_para;
/******************************************************************************/
unsigned short getRawCount(MOTORController *M);
/******************************************************************************/
//初始化I2C接口或者SPI接口
void I2CandSPI_Init(void)
{ 
	AS5047P_para.mag_cpol=SPI_CPOL_Low;
	AS5047P_para.mag_cpha=SPI_CPHA_2Edge;
	AS5047P_para.mag_BrtPre=SPI_BaudRatePrescaler_4;
	
	TLE5012B_para.mag_cpol=SPI_CPOL_Low;
	TLE5012B_para.mag_cpha=SPI_CPHA_2Edge;
	TLE5012B_para.mag_BrtPre=SPI_BaudRatePrescaler_4;
	
	MA730_para.mag_cpol=SPI_CPOL_High;
	MA730_para.mag_cpha=SPI_CPHA_2Edge;
	MA730_para.mag_BrtPre=SPI_BaudRatePrescaler_4;
	
#if M1_AS5600
	I2C1_Init_();                //AS5600
	M1.cpr = AS5600_CPR;
	printf("M1:AS5600\r\n");
#elif M1_AS5047P
	SPI2_Init_(&AS5047P_para);   //AS5047P
	M1.cpr = AS5047P_CPR;
	printf("M1:AS5047P\r\n");
#elif M1_TLE5012B
	SPI2_Init_(&TLE5012B_para);  //TLE5012B
	M1.cpr = TLE5012B_CPR;
	printf("M1:TLE5012B\r\n");
#elif M1_MA730
  SPI2_Init_(&MA730_para);     //MA730
	M1.cpr = MA730_CPR;
	printf("M1:MA730\r\n");
#endif

	delay_ms(50);
	
#if M2_AS5600
	I2C2_Init_();                //AS5600
	M2.cpr = AS5600_CPR;
	printf("M2:AS5600\r\n");
#elif M2_AS5047P
	SPI2_Init_(&AS5047P_para);   //AS5047P
	M2.cpr = AS5047P_CPR;
	printf("M2:AS5047P\r\n");
#elif M2_TLE5012B
	SPI2_Init_(&TLE5012B_para);  //TLE5012B
	M2.cpr = TLE5012B_CPR;
	printf("M2:TLE5012B\r\n");
#elif M2_MA730
  SPI2_Init_(&MA730_para);     //MA730
	M2.cpr = MA730_CPR;
	printf("M2:MA730\r\n");
#endif
}
/******************************************************************************/
void MagneticSensor_Init(MOTORController *M)
{
	delay_ms(10);
	M->angle_data_prev = getRawCount(M);
	M->full_rotation_offset = 0;
	M->velocity_calc_timestamp=0;//SysTick->VAL;
	delay_ms(10);
	M->angle_prev = getAngle(M);
}
/******************************************************************************/
unsigned short getRawCount(MOTORController *M)  //获取编码器的原始值
{
	unsigned short val;
	
	if(M->str[1] == '1')
	{
#if M1_AS5600
	val = I2C_getRawCount(I2C1); 
#elif M1_AS5047P 
	val = ReadAS5047P(1, AS5047P_Angle)&0x3FFF;
#elif M1_TLE5012B
	val = ReadTLE5012B(1, TLE5012B_Angle)&0x7FFF;
#elif M1_MA730
	val = ReadMA730(1, MA730_Angle);   //高位在前，低位补0
#endif
	}
	else
	{
#if M2_AS5600
	val = I2C_getRawCount(I2C2); 
#elif M2_AS5047P 
	val = ReadAS5047P(2, AS5047P_Angle)&0x3FFF;
#elif M2_TLE5012B
	val = ReadTLE5012B(2, TLE5012B_Angle)&0x7FFF;
#elif M2_MA730
	val = ReadMA730(2,MA730_Angle);   //高位在前，低位补0
#endif
	}
	
	return val;
}
/******************************************************************************/
float getAngle(MOTORController *M)
{
	long angle_data,d_angle;
	
	angle_data = getRawCount(M);
	
	// tracking the number of rotations 
	// in order to expand angle range form [0,2PI] to basically infinity
	d_angle = angle_data - M->angle_data_prev;
	// if overflow happened track it as full rotation
	if(fabs(d_angle) > (0.8* M->cpr) ) M->full_rotation_offset += (d_angle > 0) ? -_2PI : _2PI; 
	// save the current angle value for the next steps
	// in order to know if overflow happened
	M->angle_data_prev = angle_data;
	// return the full angle 
	// (number of full rotations)*2PI + current sensor angle 
	return (M->full_rotation_offset + ( (float)angle_data / M->cpr) * _2PI);
}
/******************************************************************************/
// Shaft velocity calculation
float getVelocity(MOTORController *M)
{
	unsigned long now_us;
	float Ts, angle_now, vel;

	// calculate sample time
	now_us = _micros();
	Ts = (now_us - M->velocity_calc_timestamp)*1e-6f;
	M->velocity_calc_timestamp = now_us;
	// quick fix for strange cases (micros overflow)
	if(Ts <= 0 || Ts > 0.5) Ts = 1e-3f;

	// current angle
	angle_now = getAngle(M);
	// velocity calculation
	vel = (angle_now - M->angle_prev)/Ts;
	// save variables for future pass
	M->angle_prev = angle_now;
	return vel;
}
/******************************************************************************/


