#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 

#define KEY1   PBin(12)	
#define KEY2   PBin(13)	
#define KEY3   PBin(14)

#define BEEP   PBout(9)

void Key_Init(void);
	 				    
#endif
