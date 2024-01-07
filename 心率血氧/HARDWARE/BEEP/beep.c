/**
 *  @file           beep.h
 *  @brief          STM32F10x ��Դ������
 *  @version 1.0
 *  @author         ���佨
 *  @date           2017��01��18��
 */

#include "beep.h"


u16 BEEP_RING_CNT;

/**
 * @Description: ��Դ������
 * @note1  :Ƶ���ݶ�4KHz
 * @note2  :PWM_Hz=ʱ��(72M)/Period/Prescaler
 */ 
void Beep_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	

   //���ø�����Ϊ�����������,���TIM3 CH2 ��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 		//TIM3_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = 1000-1; 				//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =18-1; 				//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 		//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
/*	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 			*/
	TIM_OCInitStructure.TIM_Pulse =500; 					//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  				//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx	

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);   	//CH2Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 					//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);          			//�жϱ�־λ����

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);       			//������/�Ƚ�2�ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;  		//ѡ��ʱ��TIM3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //ѡ������ʽ���ȼ������ж�Ƕ�׼����йأ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1; //ѡ�������ȼ���ͬ����ʽ���ȼ�����Ӧ˳��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    		//ѡ��ʹ���ж�Դ
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3, DISABLE);  								//ʹ��TIM  
}

void TIM3_IRQHandler(void)
{
	static u16 TIM3_CNT;
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM3_CNT++;
	if(TIM3_CNT>BEEP_RING_CNT)
	{
		TIM_Cmd(TIM3,DISABLE);
		TIM3_CNT=0;
		BEEP_RING_CNT=0;
	}
}
/**
 * @Description: ��Դ�����������ms
 * @note1  :Ƶ���ݶ�4KHz
 */
void Beep_Ring_ms(u16 ring_ms)
{
 	BEEP_RING_CNT=ring_ms*4;  //��Ƶ���й�
 	TIM_Cmd(TIM3, ENABLE);  
}
