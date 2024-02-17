/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __DRIVER_CONFIG_BASE_H_
#define __DRIVER_CONFIG_BASE_H_
#include "stm32h7xx_hal.h"

//定义系统功能开关宏定义
#define CONFIG_FUNC_DISABLE                             0X00
#define CONFIG_FUNC_ENABLE                              0X01

/*---------------------------------------------协议板号与错误代码板号---------------------------------------------*/
//自身模块号
#define SELF_MODULE_NO                                  BoardIdentityPortGet()
//自身板号
#define SELF_BOARD_ID                                   0X1A

/*-------------------------------------导入操作系统外部变量----------------------------------*/
//操作系统最小周期
#define THREAD_X_TIME_CYCLE_MIN_MS                      1
//将延时时间转换为OSTICK
#define MS_TO_OS_TICK(ms)                               (((ms)/THREAD_X_TIME_CYCLE_MIN_MS))
//OSTICK转换为毫秒
#define OS_TICK_TO_MS(tick)                             ((tick)*THREAD_X_TIME_CYCLE_MIN_MS)


#endif






