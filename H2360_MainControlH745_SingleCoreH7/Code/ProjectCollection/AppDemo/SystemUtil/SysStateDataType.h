/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 16:20:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 10:38:24 +0800
************************************************************************************************/ 
#ifndef __SYS_STATE_DATA_TYPE_
#define __SYS_STATE_DATA_TYPE_
#include "SysStateMacro.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_STATE_INIT_FLAG              0XA55A5AA5

//系统心跳状态
typedef struct SYS_STATE_HEART
{
    //当前客户端联机状态
    CLIENT_CONNECT clientConnectState;
    //上一次接收到数据的时间
    int32_t lastRecvDataTimeStamp;
}SYS_STATE_HEART;
#define LENGTH_SYS_STATE_HEART        DIM_STRUCT_LENGTH_BYTE(SYS_STATE_HEART)


/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_STATE
{
    uint32_t stateInitFlag;//状态初始化标识
    SYS_STATE_HEART sysHeartState;//系统心跳状态
}SYS_STATE;
//参数长度
#define LENGTH_SYS_STATE     DIM_STRUCT_LENGTH_BYTE(SYS_STATE)



#endif

















