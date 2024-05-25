
#include "MyProject.h"


/************************************************
电流采样底层函数

本程序仅供学习，引用代码请标明出处
使用教程：https://blog.csdn.net/loop222/article/details/119220638
创建日期：20210905
作    者：loop222 @郑州
************************************************/
/******************************************************************************/

/******************************************************************************/
void Current_calibrateOffsets(MOTORController *M);
/******************************************************************************/
void InlineCurrentSense(MOTORController *M, float _shunt_resistor, float _gain, int _pinA, int _pinB, int _pinC)
{
	float volts_to_amps_ratio;
	
	M->pinA = _pinA;
	M->pinB = _pinB;
	M->pinC = _pinC;
	
	volts_to_amps_ratio = 1.0 /_shunt_resistor / _gain; // volts to amps
	
	M->gain_a = volts_to_amps_ratio;
	M->gain_b =-volts_to_amps_ratio;
	M->gain_c = volts_to_amps_ratio;
	
	printf("%s:gain_a:%.2f,gain_b:%.2f,gain_c:%.2f.\r\n",M->str, M->gain_a, M->gain_b, M->gain_c);
}
/******************************************************************************/
void InlineCurrentSense_Init(MOTORController *M)
{
	configureADCInline(M->pinA, M->pinB, M->pinC);
	Current_calibrateOffsets(M);   //检测偏置电压，也就是电流0A时的运放输出电压值，理论值=1.65V
}
/******************************************************************************/
// Function finding zero offsets of the ADC
void Current_calibrateOffsets(MOTORController *M)
{
	int i;
	
	M->offset_ia=0;
	M->offset_ib=0;
	M->offset_ic=0;
	// read the adc voltage 1000 times ( arbitrary number )
	for(i=0; i<1000; i++)
	{
		M->offset_ia += _readADCVoltageInline(M->pinA);
		M->offset_ib += _readADCVoltageInline(M->pinB);
		if(_isset(M->pinC)) M->offset_ic += _readADCVoltageInline(M->pinC);
		delay_ms(1);
	}
	// calculate the mean offsets
	M->offset_ia = M->offset_ia/1000;
	M->offset_ib = M->offset_ib/1000;
	if(_isset(M->pinC)) M->offset_ic = M->offset_ic / 1000;
	
	printf("%s:offset_ia:%.4f,offset_ib:%.4f,offset_ic:%.4f.\r\n",M->str, M->offset_ia, M->offset_ib, M->offset_ic);
}
/******************************************************************************/
// read all three phase currents (if possible 2 or 3)
PhaseCurrent_s getPhaseCurrents(MOTORController *M)
{
	PhaseCurrent_s current;
	
	current.a = (_readADCVoltageInline(M->pinA) - M->offset_ia)*M->gain_a;// amps
	current.b = (_readADCVoltageInline(M->pinB) - M->offset_ib)*M->gain_b;// amps
	current.c = (!_isset(M->pinC)) ? 0 : (_readADCVoltageInline(M->pinC) - M->offset_ic)*M->gain_c; // amps
	
	return current;
}
/******************************************************************************/





