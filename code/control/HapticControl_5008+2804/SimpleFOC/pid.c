

#include "MyProject.h"

/************************************************
本程序仅供学习，未经作者许可，不得用于其它任何用途
PID参数初始化、角度PID、速度PID、电流PID
使用教程：https://blog.csdn.net/loop222/article/details/119220638
创建日期：20211206
************************************************/
/******************************************************************************/
void PID_init(MOTORController *M)
{
	M->PID_vel.P=0.4;  //0.2
	M->PID_vel.I=2;    //2
	M->PID_vel.D=0;
	M->PID_vel.output_ramp=1000;
	//M->PID_vel.limit=voltage_limit;  Motor_init()函数已经对limit初始化
	M->PID_vel.error_prev=0;
	M->PID_vel.output_prev=0;
	M->PID_vel.integral_prev=0;
	M->PID_vel.timestamp_prev=_micros();
	
	M->P_ang.P=20;
	M->P_ang.I=0;
	M->P_ang.D=0;
	M->P_ang.output_ramp=0;
	//M->P_ang.limit=0;
	M->P_ang.error_prev=0;
	M->P_ang.output_prev=0;
	M->P_ang.integral_prev=0;
	M->P_ang.timestamp_prev=_micros();
	
	M->PID_q.P=0.5;  //2
	M->PID_q.I=0;
	M->PID_q.D=0;
	M->PID_q.output_ramp=0;
	//M->PID_q.limit=0;
	M->PID_q.error_prev=0;
	M->PID_q.output_prev=0;
	M->PID_q.integral_prev=0;
	M->PID_q.timestamp_prev=_micros();
	
	M->PID_d.P=0.5;  //2
	M->PID_d.I=0;
	M->PID_d.D=0;
	M->PID_d.output_ramp=0;
	//M->PID_d.limit=0;
	M->PID_d.error_prev=0;
	M->PID_d.output_prev=0;
	M->PID_d.integral_prev=0;
	M->PID_d.timestamp_prev=_micros();
}
/******************************************************************************/
// PID controller function
float PIDoperator(PIDController* PID,float error)
{
	unsigned long now_us;
	float Ts;
	float proportional,integral,derivative,output;
	float output_rate;
	
	now_us = _micros();
	Ts = (now_us - PID->timestamp_prev) * 1e-6f;
	PID->timestamp_prev = now_us;
	// quick fix for strange cases (micros overflow)
	if(Ts <= 0 || Ts > 0.5) Ts = 1e-3f;
	
	// u(s) = (P + I/s + Ds)e(s)
	// Discrete implementations
	// proportional part
	// u_p  = P *e(k)
	proportional = PID->P * error;
	// Tustin transform of the integral part
	// u_ik = u_ik_1  + I*Ts/2*(ek + ek_1)
	integral = PID->integral_prev + PID->I*Ts*0.5f*(error + PID->error_prev);
	// antiwindup - limit the output
	integral = _constrain(integral, -PID->limit, PID->limit);
	// Discrete derivation
	// u_dk = D(ek - ek_1)/Ts
	derivative = PID->D*(error - PID->error_prev)/Ts;
	
	// sum all the components
	output = proportional + integral + derivative;
	// antiwindup - limit the output variable
	output = _constrain(output, -PID->limit, PID->limit);
	
	// if output ramp defined
	if(PID->output_ramp > 0)
	{
		// limit the acceleration by ramping the output
		output_rate = (output - PID->output_prev)/Ts;
		if(output_rate > PID->output_ramp)output = PID->output_prev + PID->output_ramp*Ts;
		else if(output_rate < -PID->output_ramp)output = PID->output_prev - PID->output_ramp*Ts;
	}
	
	// saving for the next pass
	PID->integral_prev = integral;
	PID->output_prev = output;
	PID->error_prev = error;
	
	return output;
}
/******************************************************************************/

