/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 16:46:21 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_SYSTEM_H_
#define __DRIVER_CONFIG_SYSTEM_H_
#include "DriverConfigBoard.h"

/*----------------------------------------系统消息使用的内存区域---------------------------------------*/
//主消息使用的内存空间
#define MEM_REGION_SYS_MSG_MAIN                                         MEM_SRAM_IN
//CAN消息使用的内存空间         
#define MEM_REGION_SYS_MSG_CAN                                          MEM_SRAM_EXT
//串口消息使用的内存空间            
#define MEM_REGION_SYS_MSG_UART                                         MEM_SRAM_EXT
//IPC消息使用的内存空间         
#define MEM_REGION_SYS_MSG_IPC                                          MEM_SRAM_IN
//日志消息使用的内存空间            
#define MEM_REGION_SYS_MSG_LOG                                          MEM_SRAM_EXT
//系统参数使用的内存空间
#define MEM_REGION_SYS_PARAM                                            MEM_SRAM_IN
//系统状态使用的内存空间
#define MEM_REGION_SYS_STATE                                            MEM_SRAM_EXT
//服务层任务使用的内存空间          
#define MEM_REGION_SRV_TASK                                             MEM_SRAM_EXT
//模块层任务使用的内存空间          
#define MEM_REGION_MOD_TASK                                             MEM_SRAM_EXT
//应用层任务使用的内存空间          
#define MEM_REGION_APP_TASK                                             MEM_SRAM_EXT
//日志文件包含讯息          
#define SYS_LOG_INCLUDE_FILE                                            "SrvTaskHeader.h"
//printf输出            
#define SYS_LOG_Printf(format,...)                                      SystemPrintf(format, ##__VA_ARGS__)
//单独日志打印              
#define SYS_LOG_SHOW_STR(str)                                           SystemSendString(str)
//服务层IPC日志打印
#define IPC_SRV_RUNNING_LOG_SHOW                                        CONFIG_FUNC_DISABLE
//模块层IPC日志打印
#define IPC_MOD_RUNNING_LOG_SHOW                                        CONFIG_FUNC_ENABLE
//应用层IPC日志打印
#define IPC_APP_RUNNING_LOG_SHOW                                        CONFIG_FUNC_ENABLE
/********************************************各层任务时间片********************************************************/
//公共服务层时间片
#define TIME_SLICE_COMM_SRV_TASK                                        TX_NO_TIME_SLICE
//服务层时间片
#define TIME_SLICE_SRV_TASK                                             TX_NO_TIME_SLICE
//模块层时间片
#define TIME_SLICE_MOD_TASK                                             TX_NO_TIME_SLICE
//应用层层时间片
#define TIME_SLICE_APP_TASK                                             TX_NO_TIME_SLICE


#endif



















