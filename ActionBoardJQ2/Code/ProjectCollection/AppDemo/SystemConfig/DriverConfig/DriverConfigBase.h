/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-26 19:45:28 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_BASE_H_
#define __DRIVER_CONFIG_BASE_H_
#include "stm32f4xx_hal.h"
#include "SYSTEM_MACRO.h"

//定义系统功能开关宏定义
#define CONFIG_FUNC_DISABLE                             0X00
#define CONFIG_FUNC_ENABLE                              0X01

/*---------------------------------------------协议板号与错误代码板号---------------------------------------------*/
//自身模块号
#define SELF_MODULE_NO                                  SYSTEM_MODULE_NO_ACT
//自身板号
#define SELF_BOARD_ID                                   0X02

/*-------------------------------------导入操作系统外部变量----------------------------------*/
//操作系统最小周期
#define THREAD_X_TIME_CYCLE_MIN_MS                      1
//将延时时间转换为OSTICK
#define MS_TO_OS_TICK(ms)                               (((ms)/THREAD_X_TIME_CYCLE_MIN_MS))
//OSTICK转换为毫秒
#define OS_TICK_TO_MS(tick)                             ((tick)*THREAD_X_TIME_CYCLE_MIN_MS)


#endif
