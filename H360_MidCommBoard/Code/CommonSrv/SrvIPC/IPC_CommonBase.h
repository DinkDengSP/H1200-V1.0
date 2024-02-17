/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __IPC_COMMON_BASE_H_
#define __IPC_COMMON_BASE_H_
#include "DriverHeaderSystem.h"

typedef enum IPC_CMD_MACRO
{
    IPC_CMD_MACRO_COMM_TEST                         = 0xFFFFFFFF,//通讯测试
    IPC_CMD_MACRO_UTIL_ONLINE                       = 0X00000000,//联机
    IPC_CMD_MACRO_UTIL_OFFLINE                      = 0X00000001,//脱机
    IPC_CMD_MACRO_UTIL_UTC_SYNC                     = 0X00000002,//UTC同步
    IPC_CMD_MACRO_UTIL_HEART_GET                    = 0X00000003,//心跳获取
    IPC_CMD_MACRO_UTIL_EM_STOP                      = 0X00000004,//急停
    IPC_CMD_MACRO_UTIL_GET_VERSION                  = 0X00000005,//获取版本号
}IPC_CMD_MACRO;


//释放内存
void IPC_CommonFreeReturnData(IPC_RETURN_DATA* returnDataPtr);


#endif








