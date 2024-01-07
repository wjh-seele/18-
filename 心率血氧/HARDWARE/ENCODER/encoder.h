/**
 *  @file           encoder.h
 *  @brief          STM32F10x ���������ٳ���
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��24��
 */
 

#ifndef __ENCODER_H
#define __ENCODER_H	
#include "sys.h" 
#define LEFT_ENCODER  2
#define RIGHT_ENCODER 4							   
void Encoder_Left_Init(void);
void Encoder_Right_Init(void);
int Read_Encoder(u8 TIMX);
#endif 















