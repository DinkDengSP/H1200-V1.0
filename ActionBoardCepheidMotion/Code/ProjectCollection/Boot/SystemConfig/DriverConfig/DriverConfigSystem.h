/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 11:59:11 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_SYSTEM_H_
#define __DRIVER_CONFIG_SYSTEM_H_
#include "DriverConfigBoard.h"

/*----------------------------------------系统消息使用的内存区域---------------------------------------*/
//主消息使用的内存空间
#define MEM_REGION_SYS_MSG_MAIN                                         MEM_SRAM_IN
//CAN消息使用的内存空间         
#define MEM_REGION_SYS_MSG_CAN                                          MEM_SRAM_IN
//串口消息使用的内存空间            
#define MEM_REGION_SYS_MSG_UART                                         MEM_SRAM_IN
//IPC消息使用的内存空间         
#define MEM_REGION_SYS_MSG_IPC                                          MEM_SRAM_IN
//日志消息使用的内存空间            
#define MEM_REGION_SYS_MSG_LOG                                          MEM_SRAM_IN
//服务层任务使用的内存空间          
#define MEM_REGION_SRV_TASK                                             MEM_SRAM_IN
//模块层任务使用的内存空间          
#define MEM_REGION_MOD_TASK                                             MEM_SRAM_IN
//应用层任务使用的内存空间          
#define MEM_REGION_APP_TASK                                             MEM_SRAM_IN
//日志文件包含讯息          
#define SYS_LOG_INCLUDE_FILE                                            "SrvTaskHeader.h"
//printf输出            
#define SYS_LOG_Printf(format,...)                                      SystemPrintf(format, ##__VA_ARGS__)
//单独日志打印              
#define SYS_LOG_SHOW_STR(str)                                           SystemSendString(str)

/********************************************IAP相关配置********************************************************/
//强制软件保持在BOOT中
#define SYS_IAP_FORCE_IN_BOOT                                           CONFIG_FUNC_DISABLE

#endif



















