/**
 *  @file           beeo.h
 *  @brief          STM32F10x LED��
 *  @version 1.0
 *  @author         ���佨
 *  @date           2016��11��24��
 */
#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"
/*public*/
void Beep_Init(void);
void Beep_Ring_ms(u16 ring_ms);
		 				    
#endif
