#include "sys.h"
//0,��֧�� os
//1,֧�� os
#define SYSTEM_SUPPORT_OS 1 //����ϵͳ�ļ����Ƿ�֧�� OS
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}
