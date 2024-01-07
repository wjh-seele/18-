#include "timer.h"

static unsigned long RunTime=0;
/**
 * @Description: ͨ�ö�ʱ��TIM3��ʱ�жϳ�ʼ��
 * @param  Period    - �Զ���װֵ
 * @param  Prescaler - ʱ��Ԥ��Ƶ��
 * @note1 :�ж�ʱ��(s)=(Period*Prescaler)/ʱ��(72M)
 * @note1 :����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M,���ʱ����ͼ
 */
void TIM3_Int_Init(u16 Period,u16 Prescaler)
{
    TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 			//ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period 		= Period-1; 			//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler 	=Prescaler-1; 			//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up; 	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 			
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);   						//ʹ�ܻ���ʧ��ָ����TIM�ж�
	NVIC_InitStructure.NVIC_IRQChannel 					 = TIM3_IRQn;//TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 3;  		//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;	//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  								//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  										//ʹ��TIMx����							 
}
	   
/**
 * @Description: ͨ�ö�ʱ��TIM3�жϺ���
 */
void TIM3_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 			//���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM �ж�Դ 
	  	TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		RunTime++; 
	}
}

unsigned long millis(void)
{
  return RunTime;
}










