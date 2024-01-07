#include "BSP_SysTick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "queue.h"

static u8  fac_us=0;//us��ʱ������   
static u16 fac_ms=0;//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
//systick�жϷ�����,ʹ��ucosʱ�õ�

extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{

      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
        
        xPortSysTickHandler();

}

   
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӣ�������������SYSTICKʱ��Ƶ��ΪAHB/8
//����Ϊ�˼���FreeRTOS�����Խ�SYSTICK��ʱ��Ƶ�ʸ�ΪAHB��Ƶ�ʣ�
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init()
{
u32 reload;
SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//ѡ���ⲿʱ��  HCLK
fac_us=SystemCoreClock/1000000;//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
reload=SystemCoreClock/1000000;//ÿ���ӵļ������� ��λΪM  
reload*=1000000/configTICK_RATE_HZ;//����configTICK_RATE_HZ�趨���ʱ��
//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��0.233s����
fac_ms=1000/configTICK_RATE_HZ;//����OS������ʱ�����ٵ�λ   
SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   //����SYSTICK�ж�
SysTick->LOAD=reload; //ÿ1/configTICK_RATE_HZ���ж�һ��
SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   //����SYSTICK    
}
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
