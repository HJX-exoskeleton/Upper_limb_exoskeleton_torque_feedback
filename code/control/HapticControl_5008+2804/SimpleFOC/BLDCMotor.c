
#include "BLDCmotor.h"

/************************************************
main中调用的接口函数都在当前文件中
=================================================
本程序仅供学习，引用代码请标明出处
使用教程：https://blog.csdn.net/loop222/article/details/119220638
创建日期：20210905
作    者：loop222 @郑州
************************************************/
DQVoltage_s voltage;
DQCurrent_s current;

float voltage_power_supply;
/******************************************************************************/
int alignSensor(MOTORController *M);
float velocityOpenloop(MOTORController *M,float target_velocity);
float angleOpenloop(MOTORController *M,float target_angle);
/******************************************************************************/
void Motor_init(MOTORController *M)
{
	printf("%s: Init\r\n",M->str);
	
	if(M->voltage_sensor_align > M->voltage_limit) M->voltage_sensor_align = M->voltage_limit;
	
	// current control loop controls voltage
	M->PID_q.limit = M->voltage_limit;
	M->PID_d.limit = M->voltage_limit;
	// velocity control loop controls current
	// 如果是电压模式限制电压，如果是电流模式限制电流
	if(M->torque_controller == Type_voltage)M->PID_vel.limit = M->voltage_limit;  //速度模式的电流限制
	else  M->PID_vel.limit = M->current_limit;
	M->P_ang.limit = M->velocity_limit;      //角度模式的速度限制
	
	if(M->str[1] == '1')M1_Enable;
	else  M2_Enable;
	M->enable=1;
	printf("%s: Enable driver.\r\n",M->str);
}
/******************************************************************************/
void Motor_initFOC(MOTORController *M,float zero_electric_offset, Direction _sensor_direction)
{
	//int exit_flag = 1;
	
	if(zero_electric_offset!=0)
	{
    // abosolute zero offset provided - no need to align
    M->zero_electric_angle = zero_electric_offset;
    // set the sensor direction - default CW
    M->sensor_direction = _sensor_direction;
  }
	alignSensor(M);    //检测零点偏移量和极对数
	
	//shaft_angle update
	M->angle_prev = getAngle(M);  //getVelocity(),make sure velocity=0 after power on
	delay_ms(50);
	M->shaft_velocity = shaftVelocity(M);  //必须调用一次，进入主循环后速度为0
	delay_ms(5);
	M->shaft_angle = shaftAngle(M);// shaft angle
	if(M->controller==Type_angle)M->target=M->shaft_angle;//角度模式，以当前的角度为目标角度，进入主循环后电机静止
	
	delay_ms(200);
}
/******************************************************************************/
int alignSensor(MOTORController *M)
{
	long i;
	float angle;
	float mid_angle,end_angle;
	float moved;
	
	printf("%s: Align sensor.\r\n",M->str);
	
	if(M->sensor_direction == UNKNOWN)  //没有设置，需要检测
	{
		// find natural direction
		// move one electrical revolution forward
		for(i=0; i<=500; i++)
		{
			angle = _3PI_2 + _2PI * i / 500.0;
			setPhaseVoltage(M, M->voltage_sensor_align, 0,  angle);
			delay_ms(2);
		}
		mid_angle=getAngle(M);
		
		for(i=500; i>=0; i--) 
		{
			angle = _3PI_2 + _2PI * i / 500.0 ;
			setPhaseVoltage(M, M->voltage_sensor_align, 0,  angle);
			delay_ms(2);
		}
		end_angle=getAngle(M);
		setPhaseVoltage(M, 0, 0, 0);
		delay_ms(200);
		
		printf("mid_angle=%.4f\r\n",mid_angle);
		printf("end_angle=%.4f\r\n",end_angle);
		
		moved = fabs(mid_angle - end_angle);
		if((mid_angle == end_angle)||(moved < 0.01))  //相等或者几乎没有动
		{
			printf("%s: Failed to notice movement loop222.\r\n",M->str);
			if(M->str[1] == '1')M1_Disable;    //电机检测不正常，关闭驱动
			else  M2_Disable;
			M->enable=0;
			return 0;
		}
		else if(mid_angle < end_angle)
		{
			printf("%s: sensor_direction==CCW\r\n",M->str);
			M->sensor_direction=CCW;
		}
		else
		{
			printf("%s: sensor_direction==CW\r\n",M->str);
			M->sensor_direction=CW;
		}
		
		printf("%s: PP check: ",M->str);    //计算Pole_Pairs
		if( fabs(moved * M->pole_pairs - _2PI) > 0.5 )  // 0.5 is arbitrary number it can be lower or higher!
		{
			printf("fail - estimated pp:");
			M->pole_pairs=_2PI/moved+0.5;     //浮点数转整形，四舍五入
			printf("%d\r\n",M->pole_pairs);
		}
		else printf("OK!\r\n");
	}
	else
		printf("%s: Skip dir calib.\r\n",M->str);
	
	if(M->zero_electric_angle == 0)  //没有设置，需要检测
	{
		setPhaseVoltage(M, M->voltage_sensor_align, 0,  _3PI_2);  //计算零点偏移角度
		delay_ms(700);
		M->zero_electric_angle = _normalizeAngle(_electricalAngle(M->sensor_direction*getAngle(M), M->pole_pairs));
		delay_ms(20);
		printf("%s: Zero elec. angle:",M->str);
		printf("%.4f\r\n",M->zero_electric_angle);
		setPhaseVoltage(M, 0, 0, 0);
		delay_ms(200);
	}
	else
		printf("%s: Skip offset calib.\r\n",M->str);
	
	return 1;
}
/******************************************************************************/
void loopFOC(MOTORController *M)
{
	if(M->enable==0)return;   //没有使能，直接退出，减少资源占用
	if(M->controller==Type_angle_openloop || M->controller==Type_velocity_openloop) return;
	
	M->shaft_angle = shaftAngle(M);// shaft angle
	M->electrical_angle = electricalAngle(M);// electrical angle - need shaftAngle to be called first
	
	switch(M->torque_controller)
	{
		case Type_voltage:  // no need to do anything really
			break;
		case Type_dc_current:
			// read overall current magnitude
      current.q = getDCCurrent(M,M->electrical_angle);
      // filter the value values
      current.q = LPFoperator(&M->LPF_q,current.q);
      // calculate the phase voltage
      voltage.q = PIDoperator(&M->PID_q,(M->current_sp - current.q)); 
      voltage.d = 0;
			break;
		case Type_foc_current:
			// read dq currents
      current = getFOCCurrents(M,M->electrical_angle);
      // filter values
      current.q = LPFoperator(&M->LPF_q,current.q);
      current.d = LPFoperator(&M->LPF_d,current.d);
      // calculate the phase voltages
      voltage.q = PIDoperator(&M->PID_q,(M->current_sp - current.q)); 
      voltage.d = PIDoperator(&M->PID_d, -current.d);
			break;
		default:
			printf("%s: no torque control selected!",M->str);
			break;
	}
	// set the phase voltage - FOC heart function :)
  setPhaseVoltage(M, voltage.q, voltage.d, M->electrical_angle);
}
/******************************************************************************/
void move(MOTORController *M,float new_target)
{
	if(M->enable==0)return;   //没有使能，直接退出，减少资源占用
	M->shaft_velocity = shaftVelocity(M);
	
	switch(M->controller)
	{
		case Type_torque:
			if(M->torque_controller==Type_voltage)voltage.q = new_target;  // if voltage torque control
		  else
				M->current_sp = new_target; // if current/foc_current torque control
			break;
		case Type_angle:
			// angle set point
      M->shaft_angle_sp = new_target;
      // calculate velocity set point
      M->shaft_velocity_sp = PIDoperator(&M->P_ang,(M->shaft_angle_sp - M->shaft_angle));
      // calculate the torque command
      M->current_sp = PIDoperator(&M->PID_vel,(M->shaft_velocity_sp - M->shaft_velocity)); // if voltage torque control
      // if torque controlled through voltage  
      if(M->torque_controller == Type_voltage)
			{
				voltage.q = M->current_sp;
        voltage.d = 0;
      }
			break;
		case Type_velocity:
			// velocity set point
      M->shaft_velocity_sp = new_target;
      // calculate the torque command
      M->current_sp = PIDoperator(&M->PID_vel,(M->shaft_velocity_sp - M->shaft_velocity)); // if current/foc_current torque control
      // if torque controlled through voltage control 
      if(M->torque_controller == Type_voltage)
			{
        voltage.q = M->current_sp;  // use voltage if phase-resistance not provided
        voltage.d = 0;
      }
			break;
		case Type_velocity_openloop:
			// velocity control in open loop
      M->shaft_velocity_sp = new_target;
      voltage.q = velocityOpenloop(M, M->shaft_velocity_sp); // returns the voltage that is set to the motor
      voltage.d = 0;
			break;
		case Type_angle_openloop:
			// angle control in open loop
      M->shaft_angle_sp = new_target;
      voltage.q = angleOpenloop(M, M->shaft_angle_sp); // returns the voltage that is set to the motor
      voltage.d = 0;
			break;
	}
}
/******************************************************************************/
void setPhaseVoltage(MOTORController *M,float Uq, float Ud, float angle_el)
{
	float Uout;
	uint32_t sector;
	float T0,T1,T2;
	float Ta,Tb,Tc;
	
	if(Uq> M->voltage_limit)Uq= M->voltage_limit;
	if(Uq<-M->voltage_limit)Uq=-M->voltage_limit;
	if(Ud> M->voltage_limit)Ud= M->voltage_limit;
	if(Ud<-M->voltage_limit)Ud=-M->voltage_limit;
	
	if(Ud) // only if Ud and Uq set 
	{// _sqrt is an approx of sqrt (3-4% error)
		Uout = _sqrt(Ud*Ud + Uq*Uq) / voltage_power_supply;
		// angle normalisation in between 0 and 2pi
		// only necessary if using _sin and _cos - approximation functions
		angle_el = _normalizeAngle(angle_el + atan2(Uq, Ud));
	}
	else
	{// only Uq available - no need for atan2 and sqrt
		Uout = Uq / voltage_power_supply;
		// angle normalisation in between 0 and 2pi
		// only necessary if using _sin and _cos - approximation functions
		angle_el = _normalizeAngle(angle_el + _PI_2);
	}
	if(Uout> 0.577)Uout= 0.577;
	if(Uout<-0.577)Uout=-0.577;
	
	sector = (angle_el / _PI_3) + 1;
	T1 = _SQRT3*_sin(sector*_PI_3 - angle_el) * Uout;
	T2 = _SQRT3*_sin(angle_el - (sector-1.0)*_PI_3) * Uout;
	T0 = 1 - T1 - T2;
	
	// calculate the duty cycles(times)
	switch(sector)
	{
		case 1:
			Ta = T1 + T2 + T0/2;
			Tb = T2 + T0/2;
			Tc = T0/2;
			break;
		case 2:
			Ta = T1 +  T0/2;
			Tb = T1 + T2 + T0/2;
			Tc = T0/2;
			break;
		case 3:
			Ta = T0/2;
			Tb = T1 + T2 + T0/2;
			Tc = T2 + T0/2;
			break;
		case 4:
			Ta = T0/2;
			Tb = T1+ T0/2;
			Tc = T1 + T2 + T0/2;
			break;
		case 5:
			Ta = T2 + T0/2;
			Tb = T0/2;
			Tc = T1 + T2 + T0/2;
			break;
		case 6:
			Ta = T1 + T2 + T0/2;
			Tb = T0/2;
			Tc = T1 + T0/2;
			break;
		default:  // possible error state
			Ta = 0;
			Tb = 0;
			Tc = 0;
	}
	if(M->str[1] == '1')
	{
		TIM_SetCompare1(TIM2,Ta*PWM_Period);
		TIM_SetCompare2(TIM2,Tb*PWM_Period);
		TIM_SetCompare3(TIM2,Tc*PWM_Period);
	}
	else
	{
		TIM_SetCompare1(TIM3,Ta*PWM_Period);
		TIM_SetCompare2(TIM3,Tb*PWM_Period);
		TIM_SetCompare3(TIM3,Tc*PWM_Period);
	}
}
/******************************************************************************/
float velocityOpenloop(MOTORController *M,float target_velocity)
{
	unsigned long now_us;
	float Ts,Uq;
	
	now_us = _micros();
	Ts = (now_us - M->open_loop_timestamp)*1e-6f;
	M->open_loop_timestamp=now_us;
  // quick fix for strange cases (micros overflow + timestamp not defined)
  if(Ts <= 0 || Ts > 0.5) Ts = 1e-3f; 
	
	// calculate the necessary angle to achieve target velocity
  M->shaft_angle = _normalizeAngle(M->shaft_angle + target_velocity*Ts); 
	// for display purposes
  M->shaft_velocity = target_velocity;
	
	Uq = M->voltage_limit;
	// set the maximal allowed voltage (voltage_limit) with the necessary angle
  setPhaseVoltage(M, Uq, 0, _electricalAngle(M->shaft_angle, M->pole_pairs));
	
	return Uq;
}
/******************************************************************************/
float angleOpenloop(MOTORController *M,float target_angle)
{
	unsigned long now_us;
	float Ts,Uq;
	
	now_us = _micros();
	// calculate the sample time from last call
	Ts = (now_us - M->open_loop_timestamp)*1e-6f;
  M->open_loop_timestamp = now_us;
  // quick fix for strange cases (micros overflow + timestamp not defined)
  if(Ts <= 0 || Ts > 0.5) Ts = 1e-3f; 
	
	// calculate the necessary angle to move from current position towards target angle
  // with maximal velocity (velocity_limit)
  if(fabs( target_angle - M->shaft_angle ) > M->velocity_limit*Ts)
	{
    M->shaft_angle += _sign(target_angle - M->shaft_angle) * M->velocity_limit * Ts;
    M->shaft_velocity = M->velocity_limit;
  }
	else
	{
    M->shaft_angle = target_angle;
    M->shaft_velocity = 0;
  }
	
	Uq = M->voltage_limit;
	// set the maximal allowed voltage (voltage_limit) with the necessary angle
	setPhaseVoltage(M, Uq, 0, _electricalAngle(M->shaft_angle, M->pole_pairs));
	
  return Uq;
}
/******************************************************************************/



