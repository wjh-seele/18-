#include "sys.h"
//0,不支持 os
//1,支持 os
#define SYSTEM_SUPPORT_OS 1 //定义系统文件夹是否支持 OS
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}
