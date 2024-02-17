/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 11:50:12
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBase.h"
#include "SrvTaskHeader.h"
#if(SYSTEM_LOG_OUT_NET_8700_SOCKET == CONFIG_FUNC_ENABLE)
#include "SrvImplSocketNet8700.h"
#include "SocketNetUtilLAN8700.h"
#endif
#if(SYSTEM_LOG_OUT_NET_5500_SOCKET == CONFIG_FUNC_ENABLE)
#include "SrvImplSocketNet5500.h"
#include "SocketNetUtilW5500.h"
#endif

//外扩日志基础输出函数
void SrvImplBaseSysLogSendBuffer(uint8_t* dataBufferPtr,uint16_t bufferLength)
{
    //RTT日志输出
#if(SYSTEM_LOG_OUT_RTT_ENABLE == CONFIG_FUNC_ENABLE)
    MCU_RTT_SendBuffer(dataBufferPtr,bufferLength);
#endif
    //UART日志输出
#if(SYSTEM_LOG_OUT_UART_ENABLE == CONFIG_FUNC_ENABLE)
    BoardRS1Uart6_SendBuffer(dataBufferPtr,bufferLength);
#endif
    //NET 8700输出
#if(SYSTEM_LOG_OUT_NET_8700_SOCKET == CONFIG_FUNC_ENABLE)
    //8700网口输出,先检测网口是否连接
    if(SUCCESS == SocketNetUtilLAN8700CheckBaseInitOver())
    {
        //网口数据发送
        SrvImplSocketNetLAN8700LogSendBuf(dataBufferPtr,bufferLength);
        //检测设备自身日志,探针端口输出
        if((bufferLength > 10)&&(0 == UserMemCmp(dataBufferPtr,(uint8_t*)"[DMC_H743_",10)))
        {
            SrvImplSocketNetLAN8700ProbeSendBuf(dataBufferPtr,bufferLength);
        }
    }
#endif
    //NET 5500输出
#if(SYSTEM_LOG_OUT_NET_5500_SOCKET == CONFIG_FUNC_ENABLE)
    //5500网口输出
    if(SUCCESS == SocketNetUtilW5500CheckBaseInitOver())
    {
        //网口数据发送
        SrvImplSocketNetW5500LogSendBuf(dataBufferPtr,bufferLength);
        //检测设备自身日志,探针端口输出
        if((bufferLength > 10)&&(0 == UserMemCmp(dataBufferPtr,(uint8_t*)"[DMC_H743_",10)))
        {
            SrvImplSocketNetW5500ProbeSendBuf(dataBufferPtr,bufferLength);
        }
    }
#endif
}







