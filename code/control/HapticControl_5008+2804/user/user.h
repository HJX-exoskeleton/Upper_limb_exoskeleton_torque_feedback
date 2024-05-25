

#ifndef MYUSER_H
#define MYUSER_H


/******************************************************************************/
// dq current structure 
typedef struct 
{
	float d;
	float q;
} DQCurrent_s;
// dq voltage structs
typedef struct 
{
	float d;
	float q;
} DQVoltage_s;
/******************************************************************************/
typedef struct 
{
	float Tf; //!< Low pass filter time constant
	float y_prev; //!< filtered value in previous execution step 
	unsigned long timestamp_prev;  //!< Last execution timestamp
} LowPassFilter;

typedef struct 
{
	float P; //!< Proportional gain 
	float I; //!< Integral gain 
	float D; //!< Derivative gain 
	float output_ramp; //!< Maximum speed of change of the output value
	float limit; //!< Maximum output value
	float error_prev; //!< last tracking error value
	float output_prev;  //!< last pid output value
	float integral_prev; //!< last integral component value
	unsigned long timestamp_prev; //!< Last execution timestamp
} PIDController;

// phase current structure 
typedef struct 
{
	float a;
	float b;
	float c;
} PhaseCurrent_s;

typedef enum
{
	Type_voltage, //!< Torque control using voltage
	Type_dc_current, //!< Torque control using DC current (one current magnitude)
	Type_foc_current //!< torque control using dq currents
} TorqueControlType;

typedef enum
{
	Type_torque,//!< Torque control
	Type_velocity,//!< Velocity motion control
	Type_angle,//!< Position/angle motion control
	Type_velocity_openloop,
	Type_angle_openloop
} MotionControlType;

typedef enum
{
	CW      = 1,  //clockwise
	CCW     = -1, // counter clockwise
	UNKNOWN = 0   //not yet known or invalid state
} Direction;

/******************************************************************************/
typedef struct 
{
	int enable;      //零点校准如果电机不正常，enable=0，主循环中对应电机不占用资源
	char str[4];     //定义结构体的名字 "M1"或者"M2"
	
	float voltage_limit;
	float voltage_sensor_align;
	int  pole_pairs;
	float velocity_limit;
	float current_limit;
	unsigned long open_loop_timestamp;
	float target;
	
	LowPassFilter  LPF_q,LPF_d,LPF_vel;
	PIDController  PID_q,PID_d,PID_vel,P_ang;
	
	int pinA;
	int pinB;
	int pinC;
	float gain_a;
	float gain_b;
	float gain_c;
	float offset_ia;
	float offset_ib;
	float offset_ic;
	
	TorqueControlType torque_controller;
	MotionControlType controller;
	Direction sensor_direction;
	
	float zero_electric_angle;
	float shaft_angle;//!< current motor angle
	float electrical_angle;
	float shaft_velocity;
	float current_sp;
	float shaft_velocity_sp;
	float shaft_angle_sp;
	
	long  cpr;
	float full_rotation_offset;
	long  angle_data_prev;
	float angle_prev;
	unsigned long velocity_calc_timestamp;
} MOTORController;

extern MOTORController M1,M2;
/******************************************************************************/
#endif

