/**
 *  @file           encoder.h
 *  @brief          STM32F10x ���������ٳ���
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��24��
 */


#include "encoder.h" 

/* ENCODER GPIO define. */
#define ENCODER_L_GPIO_A        GPIO_Pin_0
#define ENCODER_L_GPIO_B        GPIO_Pin_1
#define ENCODER_L_GPIO          GPIOA
#define ENCODER_L_GPIO_RCC      RCC_APB2Periph_GPIOA
#define ENCODER_L_TIM_RCC		RCC_APB1Periph_TIM2		//ע����ʱ�Ӻ���
#define ENCODER_L_TIMER		    TIM2

#define ENCODER_R_GPIO_A        GPIO_Pin_6
#define ENCODER_R_GPIO_B        GPIO_Pin_7
#define ENCODER_R_GPIO          GPIOB
#define ENCODER_R_GPIO_RCC      RCC_APB2Periph_GPIOB
#define ENCODER_R_TIM_RCC 		RCC_APB1Periph_TIM4		//ע����ʱ�Ӻ���
#define ENCODER_R_TIMER		    TIM4

/**
 * @Description:��������ʼ��
 */ 
void Encoder_Left_Init(void)
{
    GPIO_InitTypeDef         GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(ENCODER_L_TIM_RCC,ENABLE);					//����TIMʱ��
 	RCC_APB2PeriphClockCmd(ENCODER_L_GPIO_RCC,ENABLE);					//����GPIOʱ��
	
	
	GPIO_InitStructure.GPIO_Pin = ENCODER_L_GPIO_A|ENCODER_L_GPIO_B;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ENCODER_L_GPIO,&GPIO_InitStructure); 

	
    TIM_TimeBaseStructure.TIM_Period = 65535; 							//������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 							//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(ENCODER_L_TIMER, &TIM_TimeBaseStructure); 
	
	//���ö�ʱ��Ϊ������ģʽ  �ı�Ƶ	 �ֲ���˵��������ģʽ������TIM_ICPolarity_BothEdge
	TIM_EncoderInterfaceConfig(ENCODER_L_TIMER, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

	TIM_ICStructInit(&TIM_ICInitStructure);
 	TIM_ICInitStructure.TIM_ICFilter = 0;  								//�����˲���
	TIM_ICInit(ENCODER_L_TIMER, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(ENCODER_L_TIMER, &TIM_ICInitStructure);
//  TIM_ClearFlag(ENCODER_L_TIMER, TIM_FLAG_Update);  	 				//������б�־λ
//  TIM_ITConfig(ENCODER_L_TIMER, TIM_IT_Update, DISABLE); 				//��ֹ�жϸ���
	TIM_Cmd(ENCODER_L_TIMER, ENABLE);  									//ʹ��TIM
}

/**
 * @Description:��������ʼ��
 */
void Encoder_Right_Init(void)
{
    GPIO_InitTypeDef         GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(ENCODER_R_TIM_RCC,ENABLE);					//����TIMʱ��
 	RCC_APB2PeriphClockCmd(ENCODER_R_GPIO_RCC,ENABLE);					//����GPIOʱ��
	
	
	GPIO_InitStructure.GPIO_Pin = ENCODER_R_GPIO_A|ENCODER_R_GPIO_B;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ENCODER_R_GPIO,&GPIO_InitStructure); 

	
    TIM_TimeBaseStructure.TIM_Period = 65535; 							//������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ ����������ó�65535  
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 							//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(ENCODER_R_TIMER, &TIM_TimeBaseStructure); 
	
	//���ö�ʱ��Ϊ������ģʽ  �ı�Ƶ	 �ֲ���˵��������ģʽ������TIM_ICPolarity_BothEdge
	TIM_EncoderInterfaceConfig(ENCODER_R_TIMER, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

	TIM_ICStructInit(&TIM_ICInitStructure);
 	TIM_ICInitStructure.TIM_ICFilter = 0;  								//�����˲���
	TIM_ICInit(ENCODER_L_TIMER, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(ENCODER_L_TIMER, &TIM_ICInitStructure);
//  TIM_ClearFlag(ENCODER_R_TIMER, TIM_FLAG_Update);  	 				//������б�־λ
//  TIM_ITConfig(ENCODER_R_TIMER, TIM_IT_Update, DISABLE);			 	//��ֹ�жϸ���
	TIM_Cmd(ENCODER_R_TIMER, ENABLE);  									//ʹ��TIM
}

/**
 * @Description: ��ȡָ���ı�����
 * @param  TIMX        - ��ʱ��
 * @return Encoder_TIM - ��������
 */ 
int Read_Encoder(u8 TIMX) 
{
	static int Encoder_TIM;
	switch(TIMX) 
	{
		case 2: Encoder_TIM= (short)TIM2 -> CNT;
				TIM2 -> CNT=0;
		break;
		case 4: Encoder_TIM= (short)TIM4 -> CNT;
				TIM4 -> CNT=0;
		break;
		default:  Encoder_TIM=0;
	}
	return Encoder_TIM;
}		



