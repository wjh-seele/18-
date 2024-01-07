/**  
*  @file           uart3_dma.h 
*  @brief          STM32F10x USART+DMA���ͺͽ��գ����ձ��浽����USART3_RECEIVE_DATA��
*  @version 1.0     
*  @author         ���佨  
*  @date           2016��11��24�� 
*/ 

#ifndef __USART3_DMA_H
#define __USART3_DMA_H
#include "sys.h" 

struct JY901_buffer
{
	unsigned char  len;   //���ݳ���
	unsigned char  buf[64];  //�������ݻ�����
};
extern struct JY901_buffer JY901_rx;

extern u8 USART3_TX_BUSY; //0������ 1:���ڷ���

void USART3_DMA_Init(u32 baud);
void USART3_DMA_Send_Once_Data(u8 *data,u16 size);
void USART3_Receive_Check(void);
#endif



