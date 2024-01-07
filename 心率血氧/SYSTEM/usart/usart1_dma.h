/**  
*  @file           uart1_dma.h 
*  @brief          STM32F10x USART+DMA���ͺͽ��գ����ձ��浽����USART1_RECEIVE_DATA��
*  @version 1.0     
*  @author         ���佨  
*  @date           2016��11��24�� 
*/ 

#ifndef __USART1_DMA_H
#define __USART1_DMA_H
#include "sys.h" 

struct uart1_buffer
{
	unsigned char  len;   //���ݳ���
	unsigned char  buf[64];  //�������ݻ�����
};
extern struct uart1_buffer uart1_rx,uart1_tx;

extern u8 USART1_TX_BUSY; //0������ 1:���ڷ���

void USART1_DMA_Init(u32 baud);
void USART1_DMA_Send_Once_Data(u8 *data,u16 size);
void USART1_Receive_Check(void);
#endif


