/**
 *  @file           control.h
 *  @brief          电机PID控制程序
 *  @version 1.0
 *  @author         李其建
 *  @date           2016年11月25日
 */


#include "control.h"	
#include "motor.h"

/*private*/



/**
 * @Description: 赋值给PWM寄存器
 * @param  lmotor - 左轮PWM
 * @param  rmotor - 右轮PWM
 */
void Set_Pwm(int lmotor, int rmotor)
{
	if(lmotor < 0)			AIN1=0,	AIN2=1,	PWMA = -lmotor;
	else if(lmotor > 0)    	AIN2=0, AIN1=1,	PWMA =  lmotor;
	else					AIN2=0, AIN1=0,	PWMA =  0;
	if(rmotor < 0)			BIN2=0,	BIN1=1,	PWMB = -rmotor;
	else if(rmotor > 0)     BIN1=0,	BIN2=1,	PWMB =  rmotor; 
	else					BIN1=0,	BIN2=0,	PWMB =  0; 
}


/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
**************************************************************************/
//u8 Turn_Off(float angle, int voltage)
//{
//	    u8 temp;
//			if(angle<-40||angle>40||1==Flag_Stop||voltage<1110)//电池电压低于11.1V关闭电机
//			{	                                                 //===倾角大于40度关闭电机
//      temp=1;                                            //===Flag_Stop置1关闭电机
//			AIN1=0;                                            
//			AIN2=0;
//			BIN1=0;
//			BIN2=0;
//      }
//			else
//      temp=0;
//      return temp;			
//}

