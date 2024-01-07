/**
 *  @file           control.h
 *  @brief          ���PID���Ƴ���
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��25��
 */


#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"



void Set_Pwm(int lmotor, int rmotor);


u8 Turn_Off(float angle, int voltage);
#endif
