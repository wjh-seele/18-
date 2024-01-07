#include "struct_all.h"
#include "delay.h"
#include "sys.h"
#include "usart1_dma.h"

#include "algorithm.h"
#include "max30102.h"

#include "myiic.h"
#include "LCD1602.h"
#include "GPIO.h"


#include "FreeRTOS.h"
#include "task.h"

#define MAX_BRIGHTNESS 255

#define START_TASK_PRIO        1                    //�������ȼ�
#define START_STK_SIZE         128              //����ջ��С    
TaskHandle_t StartTask_Handler;            //������
void start_task(void *pvParameters);//������

#define TASK1_PRIO        2                    //�������ȼ�
#define TASK1_STK_SIZE         128              //����ջ��С    
TaskHandle_t LED0Task_Handler;            //������
void task1_task(void *pvParameters);    //������

#define TASK2_TASK_PRIO        3                    //�������ȼ�
#define TASK2_STK_SIZE         128              //����ջ��С    
TaskHandle_t LED1Task_Handler;            //������
void task2_task(void *pvParameters);    //������

#define TASK3_TASK_PRIO        4                    //�������ȼ�
#define TASK3_STK_SIZE         128              //����ջ��С    
TaskHandle_t LED1Task_Handler;            //������
void task3_task(void *pvParameters);    //������

void BoardInit(void)
{
//    BSP_SysTickInit();
//    GpioInit();
//    UartInit(115200);
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����TASK1����
    xTaskCreate((TaskFunction_t )task1_task,         
                (const char*    )"TASK1",       
                (uint16_t       )TASK1_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )TASK1_PRIO,    
                (TaskHandle_t*  )&LED0Task_Handler);   
								
    //����TASK2����
    xTaskCreate((TaskFunction_t )task2_task,     
                (const char*    )"TASK2",   
                (uint16_t       )TASK2_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK2_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        

    //����TASK3����
    xTaskCreate((TaskFunction_t )task3_task,     
                (const char*    )"TASK3",   
                (uint16_t       )TASK3_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TASK3_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler); 								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}







uint32_t aun_ir_buffer[150]; //infrared LED sensor data
uint32_t aun_red_buffer[150];  //red LED sensor data
int32_t n_ir_buffer_length; //data length
int32_t n_spo2;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

int32_t hr_buf[16];
int32_t hrSum;
int32_t hrAvg;//����
int32_t spo2_buf[16];
int32_t spo2Sum;
int32_t spo2Avg;//Ѫ��Ũ��
int32_t spo2BuffFilled;
int32_t hrBuffFilled;
int32_t hrValidCnt = 0;
int32_t spo2ValidCnt = 0;
int32_t hrThrowOutSamp = 0;
int32_t spo2ThrowOutSamp = 0;
int32_t spo2Timeout = 0;
int32_t hrTimeout = 0;

int32_t COUNT = 0;
int32_t heartrateMax = 120;//��������
int32_t SpO2Min = 80,SpO2Max = 100;//Ѫ��Ũ��������
int8_t setn = 0;//���ñ�־


int main(void)
{
    delay_init();	    		//��ʱ������ʼ��
    NVIC_Configuration();
	  delay_ms(500);
    LCD_Init();//1602��ʼ��
    Key_Init();//������ʼ��
    LCD_Write_String(0,0,"HR:---  SpO2:---");
		LCD_Write_String(0,1,"                ");
    bsp_InitI2C();//IIC��ʼ��
    maxim_max30102_reset(); //resets the MAX30102

    //pinMode(2, INPUT);  //pin D2 connects to the interrupt output pin of the MAX30102

    maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_dummy); //Reads/clears the interrupt status register
    maxim_max30102_init();  //initialize the MAX30102
	
	//  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    BoardInit();
     
    //������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();   

								
								
								
								
    while(1)
    {

    }
}


//task1������ 
void task1_task(void *pvParameters)
{
    while(1)
    {
      {
						uint32_t un_min, un_max, un_prev_data, un_brightness;  //variables to calculate the on-board LED brightness that reflects the heartbeats
						int32_t i;
						float f_temp;

						un_brightness = 0;
						un_min = 0x3FFFF;
						un_max = 0;

						n_ir_buffer_length = 150; //buffer length of 150 stores 3 seconds of samples running at 50sps

						//read the first 150 samples, and determine the signal range
						for(i = 0; i < n_ir_buffer_length; i++)
						{
								//while(KEY0 == 1); //wait until the interrupt pin asserts
								maxim_max30102_read_fifo((aun_ir_buffer+i), (aun_red_buffer+i));  //�°汾

								if(un_min > aun_red_buffer[i])
										un_min = aun_red_buffer[i]; //update signal min
								if(un_max < aun_red_buffer[i])
										un_max = aun_red_buffer[i]; //update signal max
						}
						un_prev_data = aun_red_buffer[i];
						//calculate heart rate and SpO2 after first 150 samples (first 3 seconds of samples)
						maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

						//Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
						while(1)
						{
								i = 0;
								un_min = 0x3FFFF;
								un_max = 0;
								void task3_task();
								if(setn == 0)
								{
										//dumping the first 50 sets of samples in the memory and shift the last 100 sets of samples to the top
										for(i = 50; i < 150; i++)
										{
												aun_red_buffer[i - 50] = aun_red_buffer[i];
												aun_ir_buffer[i - 50] = aun_ir_buffer[i];

												//update the signal min and max
												if(un_min > aun_red_buffer[i])
														un_min = aun_red_buffer[i];
												if(un_max < aun_red_buffer[i])
														un_max = aun_red_buffer[i];
										}

										//take 50 sets of samples before calculating the heart rate.
										for(i = 100; i < 150; i++)
										{
												un_prev_data = aun_red_buffer[i - 1];
												//while(KEY0 == 1);
												maxim_max30102_read_fifo((aun_ir_buffer+i), (aun_red_buffer+i));  //�°汾

												//calculate the brightness of the LED
												if(aun_red_buffer[i] > un_prev_data)
												{
														f_temp = aun_red_buffer[i] - un_prev_data;
														f_temp /= (un_max - un_min);
														f_temp *= MAX_BRIGHTNESS;
														f_temp = un_brightness - f_temp;
														if(f_temp < 0)
																un_brightness = 0;
														else
																un_brightness = (int)f_temp;
												}
												else
												{
														f_temp = un_prev_data - aun_red_buffer[i];
														f_temp /= (un_max - un_min);
														f_temp *= MAX_BRIGHTNESS;
														un_brightness += (int)f_temp;
														if(un_brightness > MAX_BRIGHTNESS)
																un_brightness = MAX_BRIGHTNESS;
												}
										}
										maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
										if(COUNT++ > 10)
										{
													COUNT = 0;
													if ((ch_hr_valid == 1) && (n_heart_rate < 150) && (n_heart_rate > 60))
													{
															hrTimeout = 0;

															// Throw out up to 1 out of every 5 valid samples if wacky
															if (hrValidCnt == 4)
															{
																	hrThrowOutSamp = 1;
																	hrValidCnt = 0;
																	for (i = 12; i < 16; i++)
																	{
																			if (n_heart_rate < hr_buf[i] + 10)
																			{
																					hrThrowOutSamp = 0;
																					hrValidCnt   = 4;
																			}
																	}
															}
															else
															{
																	hrValidCnt = hrValidCnt + 1;
															}

															if (hrThrowOutSamp == 0)
															{

																	// Shift New Sample into buffer
																	for(i = 0; i < 15; i++)
																	{
																			hr_buf[i] = hr_buf[i + 1];
																	}
																	hr_buf[15] = n_heart_rate;

																	// Update buffer fill value
																	if (hrBuffFilled < 16)
																	{
																			hrBuffFilled = hrBuffFilled + 1;
																	}

																	// Take moving average
																	hrSum = 0;
																	if (hrBuffFilled < 2)
																	{
																			hrAvg = 0;
																	}
																	else if (hrBuffFilled < 4)
																	{
																			for(i = 14; i < 16; i++)
																			{
																					hrSum = hrSum + hr_buf[i];
																			}
																			hrAvg = hrSum >> 1;
																	}
																	else if (hrBuffFilled < 8)
																	{
																			for(i = 12; i < 16; i++)
																			{
																					hrSum = hrSum + hr_buf[i];
																			}
																			hrAvg = hrSum >> 2;
																	}
																	else if (hrBuffFilled < 16)
																	{
																			for(i = 8; i < 16; i++)
																			{
																					hrSum = hrSum + hr_buf[i];
																			}
																			hrAvg = hrSum >> 3;
																	}
																	else
																	{
																			for(i = 0; i < 16; i++)
																			{
																					hrSum = hrSum + hr_buf[i];
																			}
																			hrAvg = hrSum >> 4;
																	}
															}
															hrThrowOutSamp = 0;
													}
													else
													{
															hrValidCnt = 0;
															if (hrTimeout == 4)
															{
																	hrAvg = 0;
																	hrBuffFilled = 0;
															}
															else
															{
																	hrTimeout++;
															}
													}

													if ((ch_spo2_valid == 1) && (n_spo2 > 70))
													{
															spo2Timeout = 0;

															// Throw out up to 1 out of every 5 valid samples if wacky
															if (spo2ValidCnt == 4)
															{
																	spo2ThrowOutSamp = 1;
																	spo2ValidCnt = 0;
																	for (i = 12; i < 16; i++)
																	{
																			if (n_spo2 > spo2_buf[i] - 10)
																			{
																					spo2ThrowOutSamp = 0;
																					spo2ValidCnt   = 4;
																			}
																	}
															}
															else
															{
																	spo2ValidCnt = spo2ValidCnt + 1;
															}

															if (spo2ThrowOutSamp == 0)
															{

																	// Shift New Sample into buffer
																	for(i = 0; i < 15; i++)
																	{
																			spo2_buf[i] = spo2_buf[i + 1];
																	}
																	spo2_buf[15] = n_spo2;

																	// Update buffer fill value
																	if (spo2BuffFilled < 16)
																	{
																			spo2BuffFilled = spo2BuffFilled + 1;
																	}

																	// Take moving average
																	spo2Sum = 0;
																	if (spo2BuffFilled < 2)
																	{
																			spo2Avg = 0;
																	}
																	else if (spo2BuffFilled < 4)
																	{
																			for(i = 14; i < 16; i++)
																			{
																					spo2Sum = spo2Sum + spo2_buf[i];
																			}
																			spo2Avg = spo2Sum >> 1;
																	}
																	else if (spo2BuffFilled < 8)
																	{
																			for(i = 12; i < 16; i++)
																			{
																					spo2Sum = spo2Sum + spo2_buf[i];
																			}
																			spo2Avg = spo2Sum >> 2;
																	}
																	else if (spo2BuffFilled < 16)
																	{
																			for(i = 8; i < 16; i++)
																			{
																					spo2Sum = spo2Sum + spo2_buf[i];
																			}
																			spo2Avg = spo2Sum >> 3;
																	}
																	else
																	{
																			for(i = 0; i < 16; i++)
																			{
																					spo2Sum = spo2Sum + spo2_buf[i];
																			}
																			spo2Avg = spo2Sum >> 4;
																	}
															}
															spo2ThrowOutSamp = 0;
													}
													else
													{
															spo2ValidCnt = 0;
															if (spo2Timeout == 4)
															{
																	spo2Avg = 0;
																	spo2BuffFilled = 0;
															}
															else
															{
																	spo2Timeout++;
															}
													}

										}
								}
						}
				} 
								vTaskDelay(500);
    }
}   

//task2������
void task2_task(void *pvParameters)
{
    while(1)
    {
								//��ʾ��ص���Ϣ
				LCD_Write_Char(3,0,hrAvg%1000/100+'0');
				LCD_Write_Char(4,0,hrAvg%100/10+'0');
				LCD_Write_Char(5,0,hrAvg%10+'0');

				LCD_Write_Char(13,0,spo2Avg%1000/100+'0');
				LCD_Write_Char(14,0,spo2Avg%100/10+'0');
				LCD_Write_Char(15,0,spo2Avg%10+'0');
				
				if(((hrAvg!=0)&&(hrAvg>=heartrateMax)) || ((spo2Avg!=0)&&((spo2Avg<=SpO2Min) || (spo2Avg>=SpO2Max))))//���ޣ�����������
				BEEP = 1;
				else
				BEEP = 0;
        vTaskDelay(800);
    }
}

//task3������
void task3_task(void *pvParameters)
{
    while(1)
    {
								if(KEY1 == 0)
				{
						delay_ms(10);
						if(KEY1 == 0)
						{					
								while(KEY1 == 0);
								BEEP = 0;
								setn ++;
								if(setn > 3)//���´�������3�Σ��˳�����
								{
										setn = 0;
										LCD_Write_Com(0x0C);//�رչ��
										LCD_Write_String(0,0,"HR:---  SpO2:---");
										LCD_Write_String(0,1,"                ");
								}
								if(setn == 1)//����Ѫ������Ũ�ȱ�����Χ
								{
										LCD_Write_String(0,0,"  Set The SpO2  ");
										LCD_Write_String(0,1,"    000~000%    ");
									
										LCD_Write_Char(4,1,SpO2Min%1000/100+'0');
										LCD_Write_Char(5,1,SpO2Min%100/10+'0');
										LCD_Write_Char(6,1,SpO2Min%10+'0');

										LCD_Write_Char(8,1,SpO2Max%1000/100+'0');
										LCD_Write_Char(9,1,SpO2Max%100/10+'0');
										LCD_Write_Char(10,1,SpO2Max%10+'0');
									
										LCD_Write_Com(0x80+0x40+6);
										LCD_Write_Com(0x0f);
								}
								if(setn == 2)
								{
										LCD_Write_Com(0x80+0x40+10);
										LCD_Write_Com(0x0f);
								}
								if(setn == 3)//�������ʱ�������
								{
										LCD_Write_String(0,0,"  Set The Heart ");
										LCD_Write_String(0,1,"       000      ");
									
										LCD_Write_Char(7,1,heartrateMax%1000/100+'0');
										LCD_Write_Char(8,1,heartrateMax%100/10+'0');
										LCD_Write_Char(9,1,heartrateMax%10+'0');
									
										LCD_Write_Com(0x80+0x40+9);
										LCD_Write_Com(0x0f);
								}
					}
				}
				if(KEY2 == 0)//�Ӽ�
				{
						delay_ms(10);
						if(KEY2 == 0)
						{
								while(KEY2 == 0);
								if(setn == 1)
								{
										if(SpO2Max - SpO2Min > 1)SpO2Min++;//���ޱ���������ޣ����޲��ܼ�
									
										//��ʾ
										LCD_Write_Char(4,1,SpO2Min%1000/100+'0');
										LCD_Write_Char(5,1,SpO2Min%100/10+'0');
										LCD_Write_Char(6,1,SpO2Min%10+'0');
									
										LCD_Write_Com(0x80+0x40+6);
										LCD_Write_Com(0x0f);//�����˸
								}
								if(setn == 2)
								{
										if(SpO2Max < 200)SpO2Max++;//���200
									
										LCD_Write_Char(8,1,SpO2Max%1000/100+'0');
										LCD_Write_Char(9,1,SpO2Max%100/10+'0');
										LCD_Write_Char(10,1,SpO2Max%10+'0');
									 
										LCD_Write_Com(0x80+0x40+10);
										LCD_Write_Com(0x0f);
								}
								if(setn == 3)
								{
										if(heartrateMax < 200)heartrateMax++;//���200
									
										LCD_Write_Char(7,1,heartrateMax%1000/100+'0');
										LCD_Write_Char(8,1,heartrateMax%100/10+'0');
										LCD_Write_Char(9,1,heartrateMax%10+'0');
									
										LCD_Write_Com(0x80+0x40+9);
										LCD_Write_Com(0x0f);
								}
						}
				}
				if(KEY3 == 0)//����
				{
						delay_ms(10);
						if(KEY3 == 0)
						{
								while(KEY3 == 0);
								if(setn == 1)
								{
										if(SpO2Min > 0)SpO2Min--;//��С��0
									
										LCD_Write_Char(4,1,SpO2Min%1000/100+'0');
										LCD_Write_Char(5,1,SpO2Min%100/10+'0');
										LCD_Write_Char(6,1,SpO2Min%10+'0');
									
										LCD_Write_Com(0x80+0x40+6);
										LCD_Write_Com(0x0f);
								}
								if(setn == 2)
								{
										if(SpO2Max - SpO2Min > 1)SpO2Max--;//���ޱ���������ޣ����޲��ܼ�
									
										LCD_Write_Char(8,1,SpO2Max%1000/100+'0');
										LCD_Write_Char(9,1,SpO2Max%100/10+'0');
										LCD_Write_Char(10,1,SpO2Max%10+'0');
									 
										LCD_Write_Com(0x80+0x40+10);
										LCD_Write_Com(0x0f);
								}
								if(setn == 3)
								{
										if(heartrateMax > 0)heartrateMax--;//��С��0
									
										LCD_Write_Char(7,1,heartrateMax%1000/100+'0');
										LCD_Write_Char(8,1,heartrateMax%100/10+'0');
										LCD_Write_Char(9,1,heartrateMax%10+'0');
									
										LCD_Write_Com(0x80+0x40+9);
										LCD_Write_Com(0x0f);
								}
						}
				}
						vTaskDelay(800);
    }
}
