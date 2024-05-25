#ifndef MAGNETICSENSORSPI_LIB_H
#define MAGNETICSENSORSPI_LIB_H


/******************************************************************************/
#define  AS5047P_CPR      16384      //12bit
#define  TLE5012B_CPR     32768      //14bit
#define  MA730_CPR        65536      //14bit, 但高位在前,低位补0

#define  AS5047P_Angle    0xFFFF     //
#define  TLE5012B_Angle   0x8020     //command + reg,no safty word
#define  MA730_Angle      0x0000     //
/******************************************************************************/
unsigned short ReadAS5047P(int n, unsigned short  addr);
unsigned short ReadTLE5012B(int n, unsigned short Comm);
unsigned short ReadMA730(int n, unsigned short  addr);
/******************************************************************************/

#endif
