/*---------------------------------------------------------------*
*struct_all.c for AFC V0.2
*
*����ȫ�ֱ����ı��棬���еĹؼ���ȫ�ֱ����������ﶨ��
*ͨ������ֻ��һ���߳�д�룬һ�������߳������ȡ
*----------------------------------------------------------------*/


#include "struct_all.h"

/* оƬ�����Ϣ */
struct _mcu	 mcu_info;

/* ϵͳ�����Ϣ */
struct _system_info system_info;

// /* �����㷨 ��PID����*/
_pid PID_Left_Speed,PID_Right_Speed;

/*motor*/
struct _motor left_motor,right_motor;

