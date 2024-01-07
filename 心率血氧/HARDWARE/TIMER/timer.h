/**
 *  @file           timer.h
 *  @brief          STM32F10x ��ʱ�жϳ���
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��25��
 */


#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
/**
 * *FLAG_TIM3��ʱ��3�ж�״̬
 *  0-main��δ�ڴ�������
 *  1-��ʱ�жϷ�����main���Դ�������
 * -1-main�������ݳ�ʱ
 */
extern s8 FLAG_TIM3;
void TIM3_Int_Init(u16 Period,u16 Prescaler);
unsigned long millis(void); 
#endif
