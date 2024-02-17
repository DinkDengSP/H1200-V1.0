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
#include "SrvImplBase.h"
#include "SrvTaskHeader.h"

//外扩主端口消息处理程序
void SrvImplExternSocket5500MainMsgProcess(NET_RECV_PACK* netPackPtr)
{
    asm("nop");
}

//外扩日志端口消息处理程序
void SrvImplExternSocket5500LogMsgProcess(NET_RECV_PACK* netPackPtr)
{
    asm("nop");
}

//外扩探针端口消息处理程序
void SrvImplExternSocket5500ProbeMsgProcess(NET_RECV_PACK* netPackPtr)
{
    asm("nop");
}

//外扩消息处理程序
void SrvImplExternSocket5500AckSendMsgProcess(void* socketAckMsgSendPtr)
{
    asm("nop");
}

//扩展定时处理程序
void SrvImplSocketNetW5500TimerPeriod(uint32_t currentTimeTick)
{
    asm("nop");
}







