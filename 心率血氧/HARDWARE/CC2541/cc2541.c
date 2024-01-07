



#include "cc2541.h"
#include "delay.h"
#include "USART2_DMA.h"
#include "USART1_DMA.h"
#include "led.h"
#include "struct_all.h"
/**
 * @Description: CC2541���Ӽ�����
 * @note1 :������ģ�鲻���������ڴ˳����У�
 */
void CC2541_Connected_Check(void)
{
Line1:
	USART2_DMA_Send_Once_Data("AT+INQ\r\n",8);  //���������豸
/*
 * �����豸��������
 * OK\r\n       4
 * +INQS\r\n     7
 * +INQ:1 0x0015830066D9\r\n  23
 * +INQE\r\n    7
 * Devices Found 1\r\n  17

 * �����豸��������
 * OK
 * +INQS
 * +INQE
 * Devices Found 0
 * ���Ѿ��������ˣ�����at+inq��arduino�ظ�	һ������  ��+Connected 9\r\r\n��	
 * Ҫ��Arduino�����������յ� AT+INQ\r\n  ���� ��+Connected 9\r\r\n��
 */
	delay_ms(200);
	uart2_rx.len=0;
	USART2_DMA_Send_Once_Data("AT+INQ\r\n",8);  //���������豸
	delay_ms(200);
	if(uart2_rx.len>=4) 
	{	
		if(uart2_rx.buf[uart2_rx.len-4]=='1')	 //δ���ӣ�����һ���������豸
		{
			uart2_rx.len=0;
			USART2_DMA_Send_Once_Data("at+conn1\r\n",10);	 //δ����
			delay_ms(300);
			if(uart2_rx.len>=20 &&  uart2_rx.buf[uart2_rx.len-19]=='d')	 //���ӳɹ�
			{
				uart2_rx.len=0;
				USART1_DMA_Send_Once_Data("����������\r\n",12); 
			}
			else
			{
				uart2_rx.len=0;
				USART1_DMA_Send_Once_Data("�����ٴγ�������\r\n",20);
				goto Line1;
			} 	
		}
		else if(uart2_rx.buf[uart2_rx.len-4]=='0')//δ���ӣ���û�п������豸
		{
			USART1_DMA_Send_Once_Data("�����һ������\r\n",18);  
			LED=!LED;
			uart2_rx.len=0;
			goto Line1;
		}										  
		else if(uart2_rx.buf[uart2_rx.len-4]=='9') 	   //������
		{
			USART1_DMA_Send_Once_Data("����������\r\n",12);  
			uart2_rx.len=0;
		}  
	}
	else//С������ģ��û�����ӡ�
	{
		USART1_DMA_Send_Once_Data("С������ģ��û������\r\n",22);
		uart2_rx.len=0;  
		goto Line1;
	}
}

/**
 * @Description: CC2541���ݷ��ͺ�����������uart2��
 * @param  encoder_left - ���ֱ�����ֵ		
 * @param  encoder_right- ���ֱ�����ֵ
 * @param  wzL  - z����ٶȵ��ֽ�
 * @param  wzH  - z����ٶȸ��ֽ�
 * @param  YawL - z�᷽��ǵ��ֽ�
 * @param  YawH - z�᷽��Ǹ��ֽ�
 * @note1  :����Э��-0X55 0XAA  leL leH reL reH  wzL wzH  HyL HyH 0X0D
 * @note1  :ע����ٶȺʹų��Ƕ�û��ת��Ϊʵ��ֵ
 */
 void CC2541_Send_To_Arduino(short encoder_left,short encoder_right,unsigned char wzL,unsigned char wzH,unsigned char YawL,unsigned char YawH )
 {
	
	 uart2_tx.buf[2]=(unsigned char)((encoder_left& 0x00FF));
	 uart2_tx.buf[3]=(unsigned char)((encoder_left& 0xFF00)>>8);
	 
	 uart2_tx.buf[4]=(unsigned char)((encoder_right& 0x00FF));
	 uart2_tx.buf[5]=(unsigned char)((encoder_right& 0xFF00)>>8);

	 uart2_tx.buf[6]=wzL;
	 uart2_tx.buf[7]=wzH;
	 uart2_tx.buf[8]=YawL;
	 uart2_tx.buf[9]=YawH;
	 USART2_DMA_Send_Once_Data(uart2_tx.buf,11);  
 }

/**
 * @Description: CC2541���ݽ��մ���
 * @note1  :��������Э��-0X55 leL leH reL reH 0X0D
 */
void CC2541_Receive_Check(void)
{
	if(uart2_rx.len==6)
	{
		if(uart2_rx.buf[0]==0X55)
		{
			left_motor.target_speed=(short)(uart2_rx.buf[2]<<8 | uart2_rx.buf[1]);
			right_motor.target_speed=(short)(uart2_rx.buf[4]<<8 | uart2_rx.buf[3]);
		}	   
	}
	uart2_rx.len=0;
}



