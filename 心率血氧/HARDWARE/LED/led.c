/**
 *  @file           led.h
 *  @brief          STM32F10x LED��
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��24��
 */

#include "led.h"
/**
 * @Description: LED IO��ʼ��  PC13���
 */
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //�˿������
}
 
