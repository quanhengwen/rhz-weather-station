#include "includes.h"
////////////////////////////////////////////////////////////////////////////////// 
//IO操作
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司 
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////    
/***************************************
*@DESCRIPTION: --初始化PB12 13 14为输出口.并使能这两个口的时钟		    
*
*@Input: --无
*
*@Out: --无
*
*@Return: --int
****************************************/	

void IO_Init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
	RCC->APB2ENR|=1<<6;    //使能PORTE时钟	
	   	 
	GPIOB->CRH&=0XF000FFFF; 
	GPIOB->CRH|=0X03330000;//PB12 13 14推挽输出 
	
  GPIOB->ODR|=1<<12;      //PB12 输出高
	GPIOB->ODR|=1<<13;      //PB13 输出高
	GPIOB->ODR|=1<<14;      //PB14 输出高										  

}

void IO_Set(void)
{

}






