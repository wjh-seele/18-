/**
 *  @file           cc2541.h
 *  @brief          ����CC2541����������Գ���
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��25��
 */


#ifndef __CC2541_H
#define __CC2541_H	 

void CC2541_Connected_Check(void);
void CC2541_Receive_Check(void);
void CC2541_Send_To_Arduino(short encoder_left,short encoder_right,unsigned char wzL,unsigned char wzH,unsigned char YawL,unsigned char YawH );	 				    
#endif
