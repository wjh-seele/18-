 //���ʹ�� os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h" //os ʹ��
#endif

#include "usart1_dma.h"
#include "sys.h"
#include "stdio.h"	
#include <string.h>
/* UART GPIO define. */
#define USART1_GPIO_TX       GPIO_Pin_9
#define USART1_GPIO_RX       GPIO_Pin_10
#define USART1_GPIO          GPIOA
#define USART1_GPIO_RCC      RCC_APB2Periph_GPIOA

#define USART1_DATA_LEN  64  //���պͷ������ݵ���󳤶�
/*private*/
u8 USART1_SEND_DATA[USART1_DATA_LEN];  
u8 USART1_RECEIVE_DATA[USART1_DATA_LEN]; 
u8 USART1_TX_BUSY=0; //0������ 1:���ڷ���
/*public*/
struct uart1_buffer uart1_rx,uart1_tx;
	  
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


/**
* @Description: UART1_DMA��ʼ��
* @param baud - ���ڲ�����.    
*/ 
void USART1_DMA_Init(u32 baud)  
{
	//�����жϽṹ��  
    NVIC_InitTypeDef NVIC_InitStructure ;  
    //����IO��ʼ���ṹ��  
    GPIO_InitTypeDef GPIO_InitStructure;  
    //���崮�ڽṹ��    
    USART_InitTypeDef USART_InitStructure;  
    //����DMA�ṹ��  
    DMA_InitTypeDef DMA_InitStructure;  

/*����IO����*/
    //��2������GPIO��USART1������ʱ��
	RCC_APB2PeriphClockCmd(USART1_GPIO_RCC | RCC_APB2Periph_AFIO, ENABLE);	
	//��2������USART1 Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = USART1_GPIO_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);

	//��3������USART1 Rx��GPIO����Ϊ��������ģʽ
	//����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
	//���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	GPIO_InitStructure.GPIO_Pin = USART1_GPIO_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);


/*�������ڲ�������*/
 	//�򿪴��ڶ�Ӧ������ʱ��    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��У��ʱ,У��λ��������λ��,���9������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	//CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
  	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������ 
  	USART_ClearFlag(USART1, USART_FLAG_TC);     //�巢����ɱ�־��Transmission Complete flag 
/*�����ж�����*/
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);  
    USART_ITConfig(USART1,USART_IT_TXE,DISABLE); 
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//�򿪿����ж� 

  	//Enable USART1 DMA Rx Tx request 
  	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); 
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);



/*���ڷ���DMA����*/    
    //����DMAʱ�� 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMAͨ������
  	DMA_DeInit(DMA1_Channel4);
	   
  	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);	 	//�����ַ
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_SEND_DATA;		//�ڴ��ַ																	
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						//DMA���䷽��												
  	DMA_InitStructure.DMA_BufferSize = USART1_DATA_LEN;						//����DMA�ڴ���ʱ�������ĳ���
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//����DMA���������ģʽ��һ������ 	
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//����DMA���ڴ����ģʽ 	
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���������ֳ�	 
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�ڴ������ֳ�	 
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//����DMA�Ĵ���ģʽ
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//����DMA�����ȼ���
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  	DMA_Init(DMA1_Channel4, &DMA_InitStructure);							//����DMA��ͨ��
  
  	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);						    //ʹ�ܴ�������ж� 

    DMA_Cmd(DMA1_Channel4, DISABLE);
	//DMA�����ж����� 
 	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
   
   
/*���ڽ���DMA����*/    
    //����DMAʱ�� 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMAͨ������ 
  	DMA_DeInit(DMA1_Channel5);  
  	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&USART1->DR);		//�����ַ
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RECEIVE_DATA;	//�ڴ��ַ
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//DMA���䷽��
  	DMA_InitStructure.DMA_BufferSize =USART1_DATA_LEN;						//����DMA�ڴ���ʱ�������ĳ���
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//����DMA���������ģʽ��һ������  
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//����DMA���ڴ����ģʽ 
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//���������ֳ�
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		    //�ڴ������ֳ�
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//����DMA�Ĵ���ģʽ
  	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//����DMA�����ȼ���
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  	DMA_Init(DMA1_Channel5, &DMA_InitStructure);							//����DMA��ͨ��   
  
  	DMA_Cmd(DMA1_Channel5, ENABLE);
}
/**
* @Description:UART1_DMA���ⷢ������
* @param data - �������ݴ�ŵ�ַ
* @param size - ���������ֽ���    
*/  
void USART1_DMA_Send_Once_Data(u8 *data,u16 size) 
{
	//�ȴ�����  
	while (USART1_TX_BUSY);
	USART1_TX_BUSY = 1;
	//��������  
	memcpy(USART1_SEND_DATA,data,size);
	//�ı�datasizeǰ��Ҫ��ֹͨ������
	DMA_Cmd(DMA1_Channel4, DISABLE);
	//DMA1,����������
	DMA1_Channel4->CNDTR=size;
	DMA_Cmd(DMA1_Channel4, ENABLE);
}
 
/**
* @Description:���ڴ����������ж� 
* @param  buf - ���յ����� 
* @return len - ���յ����ݳ���   
*/   
static u8 USART1_RX_Finish_IRQ(u8 *buf) 
{
	u16 len = 0;
	
	//��USART_IT_IDLE��־
	USART1->SR;
	USART1->DR;
	//�ر�DMA,��ֹ�������������	  
	DMA_Cmd(DMA1_Channel5, DISABLE);
	//�����־λ 
	DMA_ClearITPendingBit(DMA1_IT_TC5);	
	//��ý���֡֡�� 
	len=USART1_DATA_LEN-DMA_GetCurrDataCounter(DMA1_Channel5);
	memcpy(buf,USART1_RECEIVE_DATA,len);
	//���ô������ݳ���  	
	DMA_SetCurrDataCounter(DMA1_Channel5,USART1_DATA_LEN);
	//��DMA,������,�ؿ�DMA  
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	return len;
}
 
/**
* @Description: �����жϴ�����   ����ʹ�ñ��ļ������������ڰ����ŵ�stm32f4xx_it.c��
*/ 
void USART1_IRQHandler(void)
{
	//��������жϴ���  
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET) 
	{
		//�رշ�������ж�  
		USART_ITConfig(USART1,USART_IT_TC,DISABLE);
		//�������  
		USART1_TX_BUSY = 0;
	}
	//��������жϴ��� 
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) 
	{
		uart1_rx.len = USART1_RX_Finish_IRQ(uart1_rx.buf);
	}
}

/*
* @Description: USARTʹ��DMA�������жϷ������ 
*/
void DMA1_Channel4_IRQHandler(void) 
{
	if(DMA_GetITStatus(DMA1_IT_TC4) != RESET) 
	{
		//�����־λ  
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		//�ر�DMA  
		DMA_Cmd(DMA1_Channel4, DISABLE);
		//�򿪷�������ж�,������������ֽ�  
		USART_ITConfig(USART1,USART_IT_TC,ENABLE);
	}
}

/**
* @Description: ���ڽ��ռ�鲢������  
*/ 
void USART1_Receive_Check(void) 
{
	if(uart1_rx.len==10) 
	{
		//Target_Left=(uart1_rx.buf[0]-48)*1000+(uart1_rx.buf[1]-48)*100+(uart1_rx.buf[2]-48)*10+(uart1_rx.buf[3]-48);
		//Target_Right=(uart1_rx.buf[4]-48)*1000+(uart1_rx.buf[5]-48)*100+(uart1_rx.buf[6]-48)*10+(uart1_rx.buf[7]-48);
		//USART1_DMA_Send_Once_Data("Receive OK\r\n",12);	
		//��һ�����if���滹������Ҫ���������
		//uart1_rx.len=0;
	}
	uart1_rx.len=0;
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/






