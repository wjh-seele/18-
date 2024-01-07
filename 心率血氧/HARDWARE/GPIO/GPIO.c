#include "GPIO.h"


//按键对应IO初始化函数
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  // LCD1602 RS-RW-EN?
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //输出速度50MHZ
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //GPIOC
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}


