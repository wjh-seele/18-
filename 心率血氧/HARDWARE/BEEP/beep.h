/**
 *  @file           beeo.h
 *  @brief          STM32F10x LED灯
 *  @version 1.0
 *  @author         李其建
 *  @date           2016年11月24日
 */
#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"
/*public*/
void Beep_Init(void);
void Beep_Ring_ms(u16 ring_ms);
		 				    
#endif
