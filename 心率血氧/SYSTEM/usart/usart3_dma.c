#include "usart3_dma.h"
#include "sys.h"
#include <string.h>
/* UART GPIO define. */
#define USART3_GPIO_TX       GPIO_Pin_10
#define USART3_GPIO_RX       GPIO_Pin_11
#define USART3_GPIO          GPIOB
#define USART3_GPIO_RCC      RCC_APB2Periph_GPIOB

/*
#define RCC_APBPeriph_USART3 RCC_APB1Periph_USART3
#define USART3_TX_DMA        DMA1_Channel2
#define USART3_RX_DMA        DMA1_Channel3
#define USART3_TX_DMA_IRQn	 DMA1_Channel2_IRQn
#define USART3_DMA_RCC       RCC_AHBPeriph_DMA1

   */

#define USART3_DATA_LEN  64  //���պͷ������ݵ���󳤶�
/*private*/
u8 USART3_SEND_DATA[USART3_DATA_LEN];  
u8 USART3_RECEIVE_DATA[USART3_DATA_LEN]; 
u8 USART3_TX_BUSY=0; //0������ 1:���ڷ���
/*public*/
struct JY901_buffer JY901_rx;
	  

/**
* @Description: UART3_DMA��ʼ��
* @param baud - ���ڲ�����.    
*/ 
void USART3_DMA_Init(u32 baud)  
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
    //��2������GPIO��USART3������ʱ��
	RCC_APB2PeriphClockCmd(USART3_GPIO_RCC | RCC_APB2Periph_AFIO, ENABLE);	
	//��2������USART3 Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = USART3_GPIO_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_GPIO, &GPIO_InitStructure);

	//��3������USART3 Rx��GPIO����Ϊ��������ģʽ
	//����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
	//���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	GPIO_InitStructure.GPIO_Pin = USART3_GPIO_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;;
	GPIO_Init(USART3_GPIO, &GPIO_InitStructure);


/*�������ڲ�������*/
 	//�򿪴��ڶ�Ӧ������ʱ��    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��У��ʱ,У��λ��������λ��,���9������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(USART3, &USART_InitStructure);
	
	USART_Cmd(USART3, ENABLE);
	//CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
  	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������ 
  	USART_ClearFlag(USART3, USART_FLAG_TC);     //�巢����ɱ�־��Transmission Complete flag 
/*�����ж�����*/
  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);  
    USART_ITConfig(USART3,USART_IT_TXE,DISABLE); 
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);//�򿪿����ж� 

  	//Enable USART3 DMA Rx Tx request 
  	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); 
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);



/*���ڷ���DMA����*/    
    //����DMAʱ�� 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMAͨ������
  	DMA_DeInit(DMA1_Channel2);
	   
  	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);	 	//�����ַ
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_SEND_DATA;		//�ڴ��ַ																	
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						//DMA���䷽��												
  	DMA_InitStructure.DMA_BufferSize = USART3_DATA_LEN;						//����DMA�ڴ���ʱ�������ĳ���
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//����DMA���������ģʽ��һ������ 	
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//����DMA���ڴ����ģʽ 	
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���������ֳ�	 
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�ڴ������ֳ�	 
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//����DMA�Ĵ���ģʽ
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//����DMA�����ȼ���
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  	DMA_Init(DMA1_Channel2, &DMA_InitStructure);							//����DMA��ͨ��
  
  	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);						    //ʹ�ܴ�������ж� 

    DMA_Cmd(DMA1_Channel2, DISABLE);
	//DMA�����ж����� 
 	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
   
   
/*���ڽ���DMA����*/    
    //����DMAʱ�� 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMAͨ������ 
  	DMA_DeInit(DMA1_Channel3);  
  	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&USART3->DR);		//�����ַ
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_RECEIVE_DATA;	//�ڴ��ַ
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//DMA���䷽��
  	DMA_InitStructure.DMA_BufferSize =USART3_DATA_LEN;						//����DMA�ڴ���ʱ�������ĳ���
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//����DMA���������ģʽ��һ������  
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//����DMA���ڴ����ģʽ 
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//���������ֳ�
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		    //�ڴ������ֳ�
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//����DMA�Ĵ���ģʽ
  	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//����DMA�����ȼ���
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  	DMA_Init(DMA1_Channel3, &DMA_InitStructure);							//����DMA��ͨ��   
  
  	DMA_Cmd(DMA1_Channel3, ENABLE);
}
/**
* @Description:UART1_DMA���ⷢ������
* @param data - �������ݴ�ŵ�ַ
* @param size - ���������ֽ���    
*/  
void USART3_DMA_Send_Once_Data(u8 *data,u16 size) 
{
	//�ȴ�����  
	while (USART3_TX_BUSY);
	USART3_TX_BUSY = 1;
	//��������  
	memcpy(USART3_SEND_DATA,data,size);
	//�ı�datasizeǰ��Ҫ��ֹͨ������
	DMA_Cmd(DMA1_Channel2, DISABLE);
	//DMA1,����������
	DMA1_Channel2->CNDTR=size;
	DMA_Cmd(DMA1_Channel2, ENABLE);
}
 
/**
* @Description:���ڴ����������ж� 
* @param  buf - ���յ����� 
* @return len - ���յ����ݳ���   
*/   
static u8 USART3_RX_Finish_IRQ(u8 *buf) 
{
	u16 len = 0;
	
	//��USART_IT_IDLE��־
	USART3->SR;
	USART3->DR;
	//�ر�DMA,��ֹ�������������	  
	DMA_Cmd(DMA1_Channel3, DISABLE);
	//�����־λ 
	DMA_ClearITPendingBit(DMA1_IT_TC3);	
	//��ý���֡֡�� 
	len=USART3_DATA_LEN-DMA_GetCurrDataCounter(DMA1_Channel3);
	memcpy(buf,USART3_RECEIVE_DATA,len);
	//���ô������ݳ���  	
	DMA_SetCurrDataCounter(DMA1_Channel3,USART3_DATA_LEN);
	//��DMA,������,�ؿ�DMA  
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	return len;
}
 
/**
* @Description: �����жϴ�����   ����ʹ�ñ��ļ������������ڰ����ŵ�stm32f4xx_it.c��
*/ 
void USART3_IRQHandler(void)
{
	//��������жϴ���  
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET) 
	{
		//�رշ�������ж�  
		USART_ITConfig(USART3,USART_IT_TC,DISABLE);
		//�������  
		USART3_TX_BUSY = 0;
	}
	//��������жϴ��� 
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) 
	{
		JY901_rx.len = USART3_RX_Finish_IRQ(JY901_rx.buf);
	}
}

/*
* @Description: USARTʹ��DMA�������жϷ������ 
*/
void DMA1_Channel2_IRQHandler(void) 
{
	if(DMA_GetITStatus(DMA1_IT_TC2) != RESET) 
	{
		//�����־λ  
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		//�ر�DMA  
		DMA_Cmd(DMA1_Channel2, DISABLE);
		//�򿪷�������ж�,������������ֽ�  
		USART_ITConfig(USART3,USART_IT_TC,ENABLE);
	}
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/






