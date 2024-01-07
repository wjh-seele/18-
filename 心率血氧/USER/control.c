/**
 *  @file           control.h
 *  @brief          ���PID���Ƴ���
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��25��
 */


#include "control.h"	
#include "motor.h"

/*private*/



/**
 * @Description: ��ֵ��PWM�Ĵ���
 * @param  lmotor - ����PWM
 * @param  rmotor - ����PWM
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
�������ܣ��쳣�رյ��
��ڲ�������Ǻ͵�ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
//u8 Turn_Off(float angle, int voltage)
//{
//	    u8 temp;
//			if(angle<-40||angle>40||1==Flag_Stop||voltage<1110)//��ص�ѹ����11.1V�رյ��
//			{	                                                 //===��Ǵ���40�ȹرյ��
//      temp=1;                                            //===Flag_Stop��1�رյ��
//			AIN1=0;                                            
//			AIN2=0;
//			BIN1=0;
//			BIN2=0;
//      }
//			else
//      temp=0;
//      return temp;			
//}

