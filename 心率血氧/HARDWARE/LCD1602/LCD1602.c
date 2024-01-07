#include "LCD1602.h"
#include "delay.h"
#include "sys.h"


u8 Dao_xu(u8 data)
{
    u8 i = 0 ,temp = 0;;
	  
	  for(i = 0; i < 8; i++)
	 {
	   temp += (((data >> i) & 0x01) << (7 - i));
	 }
	 return temp;
}

/*-----------------------------------------------
  ���ƣ�LCD1602
  ��д��shifang
  �޸ģ���
  ���ݣ�ͨ����׼����̬��ʾ�ַ�
  ���Ŷ������£�1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-
              ��æ����
------------------------------------------------*/
 void  LCD_Check_Busy(void) 
 
{
  LCD1602_RS0;	
	delay_us(1);
  LCD1602_RW1;   
	delay_us(1);
  LCD1602_EN0;
	delay_us(1);
  GPIO_Write(GPIOC,0Xff);    
	delay_ms(1);
  LCD1602_EN1;    
	delay_us(100);
}

/*------------------------------------------------
              д�������
------------------------------------------------*/
 void LCD_Write_Com(unsigned char com) 
{                           
  LCD1602_RS0;	
	delay_us(1);
  LCD1602_RW0;
  delay_us(1);	
  LCD1602_EN1;                   
	delay_us(1);
  GPIO_Write(GPIOA,(GPIO_ReadOutputData(GPIOA)&0XFF00)+Dao_xu(com));               
	delay_us(100);
  LCD1602_EN0;
}

/*------------------------------------------------
              д�����ݺ���
------------------------------------------------*/
 void LCD_Write_Data(unsigned char Data) 
{                                
  LCD1602_RS1;                 
	delay_us(1);
  LCD1602_RW0;
  delay_us(1);	
  LCD1602_EN1;                    
	delay_us(1);
  GPIO_Write(GPIOA,(GPIO_ReadOutputData(GPIOA)&0XFF00)+Dao_xu(Data));
	delay_us(100);
  LCD1602_EN0;
}
/*------------------------------------------------
              д���ַ�������
------------------------------------------------*/
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     
 	}
 else 
 	{     
 	LCD_Write_Com(0xC0 + x);     
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }
/*------------------------------------------------
              д���ַ�����
------------------------------------------------*/
 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
 {     
 if (y == 0) 
 	{     
 	LCD_Write_Com(0x80 + x);     
 	}    
 else 
 	{     
 	LCD_Write_Com(0xC0 + x);     
 	}        
 LCD_Write_Data( Data);  
 }
 
 
 void LCD1602_write_long(unsigned char x,unsigned char y,u32 data,unsigned char num) 
{
    unsigned char temp[12],i = 12;
    while(i--)
    {
         temp[i] = ' ';
    }
    temp[num] = '\0';
    while(num--)
    {
      if(data || data%10)
         temp[num] = data % 10 + 0x30;
      data = data/10;
    }
   LCD_Write_String(x,y,temp);   
}

 /*------------------------------------------------
                ��������
------------------------------------------------*/
 void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 delay_ms(5);
 }

/*------------------------------------------------*/
 void LCD_Init(void) 
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);  //����GPIOA GPIOB GPIOCʱ��
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
    GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����ٶ�50MHZ
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIOA
	
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14|GPIO_Pin_15;  // LCD1602 RS-RW-EN?
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //����ٶ�50MHZ
    GPIO_Init(GPIOC, &GPIO_InitStructure);   //GPIOC
	
   LCD_Write_Com(0x38);
   delay_ms(5);  
	 LCD_Write_Com(0x38);
   delay_ms(5);   
	 LCD_Write_Com(0x38);
   delay_ms(5);  
   LCD_Write_Com(0x08);    /*��ʾ�ر�*/
   delay_ms(5); 	
   LCD_Write_Com(0x01);    /*��ʾ����*/
   delay_ms(5); 	
   LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/ 
	 delay_ms(5); 
   LCD_Write_Com(0x0C);    /*��ʾ�����������*/
	 delay_ms(5); 
}
   
