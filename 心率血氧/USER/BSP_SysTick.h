#ifndef _BSP_SYSTICK_H
#define _BSP_SYSTICK_H
#include "stm32f10x.h"
#include "stdio.h"
#include "FreeRTOS.h"

//0,��֧��os
//1,֧��os
#define SYSTEM_SUPPORT_OS        1        //����ϵͳ�ļ����Ƿ�֧��OS

void BSP_SysTickInit(void);

#endif
