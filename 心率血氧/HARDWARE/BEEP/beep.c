/**
 *  @file           beep.h
 *  @brief          STM32F10x 无源蜂鸣器
 *  @version 1.0
 *  @author         李其建
 *  @date           2017年01月18日
 */

#include "beep.h"


u16 BEEP_RING_CNT;

/**
 * @Description: 无源蜂鸣器
 * @note1  :频率暂定4KHz
 * @note2  :PWM_Hz=时钟(72M)/Period/Prescaler
 */ 
void Beep_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM3 CH2 的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 		//TIM3_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = 1000-1; 				//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =18-1; 				//设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 		//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 		//选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
/*	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 			*/
	TIM_OCInitStructure.TIM_Pulse =500; 					//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  				//根据TIM_OCInitStruct中指定的参数初始化外设TIMx	

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);   	//CH2预装载使能	 
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 					//使能TIMx在ARR上的预装载寄存器
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);          			//中断标志位清零

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);       			//允许捕获/比较2中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;  		//选择定时器TIM3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //选择抢先式优先级（与中断嵌套级别有关）
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1; //选择子优先级（同抢先式优先级的响应顺序）
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    		//选择使能中断源
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3, DISABLE);  								//使能TIM  
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
 * @Description: 无源蜂鸣器响多少ms
 * @note1  :频率暂定4KHz
 */
void Beep_Ring_ms(u16 ring_ms)
{
 	BEEP_RING_CNT=ring_ms*4;  //与频率有关
 	TIM_Cmd(TIM3, ENABLE);  
}
