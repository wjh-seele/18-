/**
 *  @file           motor.h
 *  @brief          STM32F10x ˫·ֱ����ˢ���PWM��������
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��25��
 */

#include "motor.h"

/**
 * @Description: �������IO��ʼ�� AIN1 AIN2 BIN1 BIN2 STBY
 */
 
static void Motor_GPIO_Init(void)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��PORTʱ��
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8|GPIO_Pin_11| GPIO_Pin_12;	 	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 		 				//�������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;		 					//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 					
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12 | GPIO_Pin_14;				
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 		 				//�������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;		 					//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);										
}

/**
 * @Description: ˫·���PWM���	   ע����CHx����CHxN
 * @param  Period    - �Զ���װֵ
 * @param  Prescaler - ʱ��Ԥ��Ƶ��
 * @note1  :ע����CHx����CHxN
 * @note2  :Motor_GPIO_Init()���ÿ���IO
 * @note3  :PWM_Hz=ʱ��(72M)/Period/Prescaler
 */ 
void Motor_PWM_Init(u16 Period,u16 Prescaler)
{  
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

	Motor_GPIO_Init();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  	//ʹ��GPIO����ʱ��ʹ��
	                                                                     	

   //���ø�����Ϊ�����������,���TIM1 CH1 CH3��PWM���岨��
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13|GPIO_Pin_15; 	//TIM_CH1  CH3
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//�����������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period 		= Period-1; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler 	= Prescaler-1; 		//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 			//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode 			= TIM_OCMode_PWM2; 	//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
/*	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable; //�Ƚ����ʹ��*/
	TIM_OCInitStructure.TIM_OutputNState 	= TIM_OutputNState_Enable; 
	TIM_OCInitStructure.TIM_Pulse 			= 0; 				//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCNPolarity 	= TIM_OCNPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  					//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  					//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
    TIM_CtrlPWMOutputs(TIM1,ENABLE);							//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  			//CH1Ԥװ��ʹ��
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  			//CH3Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); 						//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM1, ENABLE);  									//ʹ��TIM1  

//	DBGMCU_Config(DBGMCU_TIM1_STOP, ENABLE);
}
