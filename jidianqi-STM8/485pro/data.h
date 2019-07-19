#ifndef DATA_H
#define DATA_H

#define u16 unsigned short int
#define u8  unsigned char 
#define u32 unsigned long 

typedef struct
{
  float co2;    // CO2
  float TVOC;   //TVOC            未用
  float tvocco2;//                未用      
  float temp20; // 室外温度         
  float humi20; //   室外湿度
  float press;  //    气压
  float press_temp;   //         温度
  
  float num1;
  float num2;
  float num3;
  float num4;
  float num5;
  float num6;
  float num7;
  float num8;
  float num9;
  
  float light_bhvi;  //光照强度 
}EVNDAT;


extern short int collect_time;
extern EVNDAT Evndat;	//传感数据结构体
extern void get_data(void);
//#define u16 unsigned long
#endif