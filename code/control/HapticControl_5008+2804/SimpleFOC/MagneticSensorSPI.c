

#include "MyProject.h"

/******************************************************************************/
#define SPI2_CS1_L  GPIO_ResetBits(GPIOB, GPIO_Pin_8)      //CS1_L
#define SPI2_CS1_H  GPIO_SetBits(GPIOB, GPIO_Pin_8)        //CS1_H
#define SPI2_CS2_L  GPIO_ResetBits(GPIOB, GPIO_Pin_12)     //CS2_L
#define SPI2_CS2_H  GPIO_SetBits(GPIOB, GPIO_Pin_12)       //CS2_H
/******************************************************************************/
void delay_s(int i)
{
	while(i--);
}
/******************************************************************************/
//AS5047P、AS5048、AS5147 代码通用
unsigned short ReadAS5047P(int n, unsigned short  addr)
{
	unsigned short  u16Data;
	
	if(n==1)SPI2_CS1_L;
	else  SPI2_CS2_L;
	SPIx_ReadWriteByte(addr);
	if(n==1)SPI2_CS1_H;
	else  SPI2_CS2_H;
	delay_s(20);  //1us
	
	if(n==1)SPI2_CS1_L;
	else  SPI2_CS2_L;
	u16Data = SPIx_ReadWriteByte(0);
	if(n==1)SPI2_CS1_H;
	else  SPI2_CS2_H;
	delay_s(20);  //1us
	
	return u16Data;
}
/******************************************************************************/
#define SPI2_TX_OFF {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x40000000;}  //把PB15(MOSI)配置为浮空模式
#define SPI2_TX_ON  {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0xB0000000;}  //把PB15(MOSI)配置为复用推挽输出(50MHz)
/******************************************************************************/
unsigned short ReadTLE5012B(int n, unsigned short Comm)
{
	unsigned short u16Data;
	
	if(n==1)SPI2_CS1_L;
	else  SPI2_CS2_L;
	SPIx_ReadWriteByte(Comm);
	SPI2_TX_OFF;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();  //Twr_delay=130ns min
	u16Data = SPIx_ReadWriteByte(0xffff);
	
	if(n==1)SPI2_CS1_H;
	else  SPI2_CS2_H;
	SPI2_TX_ON;
	return(u16Data);
}
/******************************************************************************/
/******************************************************************************/
unsigned short ReadMA730(int n, unsigned short  addr)
{
	unsigned short u16Data;
	
	if(n==1)SPI2_CS1_L;
	else  SPI2_CS2_L;
	u16Data=SPIx_ReadWriteByte(addr);
	if(n==1)SPI2_CS1_H;
	else  SPI2_CS2_H;
	
	return u16Data;
}
/******************************************************************************/


