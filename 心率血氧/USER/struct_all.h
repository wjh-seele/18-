/*---------------------------------------------------------------*
*struct_all.h for 
*
*����ȫ�ֱ����ı��棬���еĹؼ���ȫ�ֱ����������ﶨ��
*ͨ������ֻ��һ���߳�д�룬һ�������߳������ȡ
*
*           **** ���нṹ��ԭ��λ�� struct.c ****
*----------------------------------------------------------------*/


#ifndef __STRUCT_ALL_H__
#define __STRUCT_ALL_H__
#include "stm32f10x.h"
/* PI */
#define PI 3.1415926535898f


/* ʱ�� */
struct _time{
	unsigned char flag;
	
	unsigned char time_zone;		//ʱ��
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char weekday;        	//day of week
	unsigned char date;			//date of month
	unsigned char month;
	unsigned char year;
	unsigned int milisecond;
};



/* MCU ��Ϣ*/
struct _mcu
{
	float temperature;
	float usage;
};
extern struct _mcu	 mcu_info;
	
/* ϵͳ��Ϣ */
struct _system_info
{
	unsigned int cycle;  //ѭ������	
	unsigned int startup_second;	
	
	struct _mcu mcu;
	struct _time time;
};
extern struct _system_info system_info;





/* �����㷨PID */
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
	unsigned char  len;   //���ݳ���
	unsigned char  buf[128];  //�������ݻ�����
};

extern struct _wifi_rx wifi_rx;


//motor
struct _motor
{
	unsigned char flag;
	signed 	 char dir;

	signed short pwm;
	float target_speed;
	float speed;		//�����ʵ���ٶ�   ��λ������/s
	float speed0;		//�����ʵ���ٶ�  ��λ������/s
	short encoder;
};
extern struct _motor left_motor,right_motor;

#endif


