/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:03:24
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SYSTEM_STATE_DATA_TYPE_
#define __SYSTEM_STATE_DATA_TYPE_
#include "SystemStateMacro.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYSTEM_STATE_INIT_FLAG              0XA55A5AA5

//系统心跳状态
typedef struct SYSTEM_STATE_HEART
{
    //当前客户端联机状态
    CLIENT_CONNECT clientConnectState;
    //当前联机的是调试软件还是应用软件
    PROT_CONNECT_FLAG connectFlag;
    //运行模式
    MODE_RUNNING runningMode;
    //模块号
    uint8_t moduleID;
    //上一次接收到数据的时间
    int32_t lastRecvDataTimeStamp;
}SYSTEM_STATE_HEART;
#define LENGTH_SYSTEM_STATE_HEART        DIM_STRUCT_LENGTH_BYTE(SYSTEM_STATE_HEART)


/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYSTEM_STATE
{
    uint32_t stateInitFlag;//状态初始化标识
    SYSTEM_STATE_HEART sysHeartState;//系统心跳状态
}SYSTEM_STATE;
//参数长度
#define LENGTH_SYSTEM_STATE     DIM_STRUCT_LENGTH_BYTE(SYSTEM_STATE)



#endif
















