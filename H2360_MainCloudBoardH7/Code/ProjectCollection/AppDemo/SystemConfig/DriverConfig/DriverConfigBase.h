/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:10:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 10:18:05 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_BASE_H_
#define __DRIVER_CONFIG_BASE_H_
#include "stm32h7xx_hal.h"
#include "SYSTEM_MACRO.h"

/*---------------------------------------------定义系统功能开关宏定义----------------------------------------*/
#define CONFIG_FUNC_DISABLE                             0X00
#define CONFIG_FUNC_ENABLE                              0X01

/*--------------------------------------------芯片内部Flash配置---------------------------------------------*/
//H7芯片一个扇区的尺寸 128KB
#define ADDR_FLASH_SECTOR_SIZE                          (0x20000)
//Flash 容量  
#define	MCU_FLASH_BANK_SIZE		                        (1*1024*1024)
//STM32 FLASH 基础地址
#define MCU_FLASH_BASE                                  0x08000000

/*-------------------------------------------协议板号与错误代码板号------------------------------------------*/
//自身模块号
#define SELF_MODULE_NO                                  SYSTEM_MODULE_NO_CLOUD
//自身板号
#define SELF_BOARD_ID                                   SYSTEM_BOARD_ID_MAIN_CONTROL_CLOUD
/*-------------------------------------------导入操作系统外部变量------------------------------------------*/
//操作系统最小周期
#define THREAD_X_TIME_CYCLE_MIN_MS                      1
//将延时时间转换为OSTICK
#define MS_TO_OS_TICK(ms)                               (((ms)/THREAD_X_TIME_CYCLE_MIN_MS))
//OSTICK转换为毫秒
#define OS_TICK_TO_MS(tick)                             ((tick)*THREAD_X_TIME_CYCLE_MIN_MS)






#endif

















