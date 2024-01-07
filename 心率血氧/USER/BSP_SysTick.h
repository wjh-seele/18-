#ifndef _BSP_SYSTICK_H
#define _BSP_SYSTICK_H
#include "stm32f10x.h"
#include "stdio.h"
#include "FreeRTOS.h"

//0,不支持os
//1,支持os
#define SYSTEM_SUPPORT_OS        1        //定义系统文件夹是否支持OS

void BSP_SysTickInit(void);

#endif
