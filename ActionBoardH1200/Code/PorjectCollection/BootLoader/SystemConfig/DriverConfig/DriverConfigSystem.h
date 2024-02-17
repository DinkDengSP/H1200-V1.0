/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:07:12
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __DRIVER_CONFIG_SYSTEM_H_
#define __DRIVER_CONFIG_SYSTEM_H_
#include "DriverConfigBoard.h"
#include "DriverConfigDevice.h"

//服务层时间片
#define TIME_SLICE_SRV_TASK                                             TX_NO_TIME_SLICE
//模块层时间片
#define TIME_SLICE_MOD_TASK                                             TX_NO_TIME_SLICE
//应用层层时间片
#define TIME_SLICE_APP_TASK                                             TX_NO_TIME_SLICE

/*----------------------------------------系统消息使用的内存区域---------------------------------------*/
//主消息使用的内存空间
#define MEM_REGION_SYS_MSG_MAIN                                         MEM_AXI_SRAM
//串口消息使用的内存空间            
#define MEM_REGION_SYS_MSG_UART                                         MEM_SDRAM
//CAN消息使用的内存空间            
#define MEM_REGION_SYS_MSG_CAN                                          MEM_SDRAM
//日志消息使用的内存空间            
#define MEM_REGION_SYS_MSG_LOG                                          MEM_SDRAM
//IPC消息使用的内存空间            
#define MEM_REGION_SYS_MSG_IPC                                          MEM_AXI_SRAM
//系统参数使用的内存空间
#define MEM_REGION_SYS_PARAM                                            MEM_SDRAM
//系统状态使用的内存空间
#define MEM_REGION_SYS_STATE                                            MEM_SDRAM
//服务层任务使用的内存空间          
#define MEM_REGION_SRV_TASK                                             MEM_AXI_SRAM
//模块层任务使用的内存空间          
#define MEM_REGION_MOD_TASK                                             MEM_AXI_SRAM
//应用层任务使用的内存空间          
#define MEM_REGION_APP_TASK                                             MEM_SDRAM
//系统参数内存
#define MEM_REGION_SYS_PARAM                                            MEM_SDRAM
//系统状态内存
#define MEM_REGION_SYS_STATE                                            MEM_SDRAM
//日志文件包含讯息          
#define SYS_LOG_INCLUDE_FILE                                            "SrvTaskHeader.h"
//printf输出            
#define SYS_LOG_Printf(format,...)                                      SystemPrintf(format, ##__VA_ARGS__)
//单独日志打印              
#define SYS_LOG_SHOW_STR(str)                                           SystemSendString(str)

/********************************************IAP相关配置********************************************************/
//强制软件保持在BOOT中
#define SYS_IAP_FORCE_IN_BOOT                                           CONFIG_FUNC_DISABLE
//IAP错误最大重试次数
#define RETRY_COUNT_MAX_IAP_WRITE_FLASH                                 3
//IAP单包长度最大值
#define IAP_DATA_PACK_SINGLE_MAX_LENGTH                                 4096

#endif








