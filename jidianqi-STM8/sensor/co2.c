#ifndef YL

#include"iostm8s103F3.h"
#include "co2.h"
#include "delay.h"
void co2_init()
{
 asm("sim");//关全局中断
   PC_DDR &=~(0X01<<4);//PC3设为输入模式
   PC_CR1 &=~(0X01<<4);//设置浮空输入
   PC_CR2 |=0X01<<4;//使能PC3外部中断

   ITC_SPR2 &= ~(0x03<<6);//软件中断优先级寄存器清0
   ITC_SPR2 |= (0x03<<6);//禁用软件优先级
   
   EXTI_CR1 &= ~(0x03<<4);//外部中断控制寄存器清零
   EXTI_CR1 |= 0x01<<4;//端口C仅上升沿触发中断
   asm("rim");
}
u8 CO2_flage=0;
u16 CO2_cnt=0,CO2_cnt_save=0;
u32 CO2temp_s=0;
void Get_CO2(float *tempfs)
{
  if(CO2_flage==0)
  {
    *tempfs=CO2_cnt_save*2.04*3.992;//单位ms  CO2 单位PPM
    CO2_cnt_save=0;
  }
  if(0!= *tempfs)
  {
    *tempfs +=0.005;
  }	
  if(*tempfs>4200 ||*tempfs<380)
    *tempfs=0;
}

#pragma vector= EXTIC_vector
 __interrupt void  EXTI_PORTC_IRQHandler(void)
{
  if((PC_IDR&(0X01<<4))&&(CO2_flage==0))
   {
     CO2_flage=1;
     EXTI_CR1 |= 0x02<<4;//端口C仅下降沿触发中断
   } 
   if(((PC_IDR&(0X01<<4))==0)&&(CO2_flage==1))
   {                                                                                                                    
     CO2_flage=0;
     if(CO2_cnt&&(CO2_cnt<491)&&(CO2_cnt>50))
     {
     CO2_cnt_save=CO2_cnt;
     CO2_cnt=0;
     }
     EXTI_CR1 |= 0x01<<4;//端口C仅上升沿触发中断
   } 
}
#endif
