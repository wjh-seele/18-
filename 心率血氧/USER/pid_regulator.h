#ifndef __PID_REGULATIOR_H
#define __PID_REGULATIOR_H	
#include "stm32f10x.h"

#include "struct_all.h"
void PID_Parameter_Init(void);							   
float PID_Regulator(float Reference, float PresentFeedback, _pid *PID_Struct);
#endif 



