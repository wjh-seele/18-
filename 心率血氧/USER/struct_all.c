/*---------------------------------------------------------------*
*struct_all.c for AFC V0.2
*
*用于全局变量的保存，所有的关键的全局变量将在这里定义
*通常他们只有一个线程写入，一个或多个线程任意读取
*----------------------------------------------------------------*/


#include "struct_all.h"

/* 芯片相关信息 */
struct _mcu	 mcu_info;

/* 系统相关信息 */
struct _system_info system_info;

// /* 控制算法 的PID参数*/
_pid PID_Left_Speed,PID_Right_Speed;

/*motor*/
struct _motor left_motor,right_motor;

