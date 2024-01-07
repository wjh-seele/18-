/**  
*  @file           uart2_dma.h 
*  @brief          STM32F10x USART+DMA���ͺͽ��գ����ձ��浽����USART2_RECEIVE_DATA��
*  @version 1.0     
*  @author         ���佨  
*  @date           2016��11��24�� 
*/ 

#ifndef __USART2_DMA_H
#define __USART2_DMA_H	
#include "sys.h" 

struct uart2_buffer
{
	unsigned char  len;   //���ݳ���
	unsigned char  buf[64];  //�������ݻ�����
};
extern struct uart2_buffer uart2_rx,uart2_tx;

extern u8 USART2_TX_BUSY; //0������ 1:���ڷ���

void USART2_DMA_Init(u32 baud);
void USART2_DMA_Send_Once_Data(u8 *data,u16 size);
void USART2_Receive_Check(void);
#endif
