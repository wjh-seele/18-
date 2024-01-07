/*---------------------------------------------------------------*
*struct_all.h for 
*
*用于全局变量的保存，所有的关键的全局变量将在这里定义
*通常他们只有一个线程写入，一个或多个线程任意读取
*
*           **** 所有结构体原型位于 struct.c ****
*----------------------------------------------------------------*/


#ifndef __STRUCT_ALL_H__
#define __STRUCT_ALL_H__
#include "stm32f10x.h"
/* PI */
#define PI 3.1415926535898f


/* 时间 */
struct _time{
	unsigned char flag;
	
	unsigned char time_zone;		//时区
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char weekday;        	//day of week
	unsigned char date;			//date of month
	unsigned char month;
	unsigned char year;
	unsigned int milisecond;
};



/* MCU 信息*/
struct _mcu
{
	float temperature;
	float usage;
};
extern struct _mcu	 mcu_info;
	
/* 系统信息 */
struct _system_info
{
	unsigned int cycle;  //循环次数	
	unsigned int startup_second;	
	
	struct _mcu mcu;
	struct _time time;
};
extern struct _system_info system_info;





/* 控制算法PID */
typedef struct
{
	unsigned char flag;
	unsigned int  count;
	
	float Kp;
	float Ki;
	float Kd;
 
  	signed short Lower_Limit_Output;     //Lower Limit for Output limitation
  	signed short Upper_Limit_Output;     //Upper Limit for Output limitation
 	float Lower_Limit_Integral;   //Lower Limit for Integral term limitation
  	float Upper_Limit_Integral;   //Lower Limit for Integral term limitation
  	float Integral;
  	float PreviousError;
}_pid;
extern _pid PID_Left_Speed,PID_Right_Speed;

//WIFI
struct _wifi_rx
{
	unsigned char  len;   //数据长度
	unsigned char  buf[128];  //接收数据缓存区
};

extern struct _wifi_rx wifi_rx;


//motor
struct _motor
{
	unsigned char flag;
	signed 	 char dir;

	signed short pwm;
	float target_speed;
	float speed;		//电机的实际速度   单位：脉冲/s
	float speed0;		//电机的实际速度  单位：脉冲/s
	short encoder;
};
extern struct _motor left_motor,right_motor;

#endif


