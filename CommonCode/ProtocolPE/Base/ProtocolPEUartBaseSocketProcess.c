/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 18:43:57 +0800
************************************************************************************************/ 
#include "ProtocolPEUartBase.h"

//PE UART SOCKET初始化标记
extern PROTOCOL_PE_UART_INIT_SET peUartInitSet;
//获取数据访问权
extern void ProtocolPEUartSocketGetDataAccessLock(void);
//释放数据访问权
extern void ProtocolPEUartSocketGetDataAccessRelease(void);

//指令ACK处理
void ProtocolPEUartSocketPackRecvProcessAckCmd(void)
{
    uint32_t elementCount = 0;
    //获取访问量
    ProtocolPEUartSocketGetDataAccessLock();
    //检查等待链表是否为空,空直接返回
    elementCount = ADT_CircularPtrListGetNodeCount(peUartInitSet.listSendCmdWaitAckPtr);
    if(elementCount == 0)
    {
        //释放访问量
        ProtocolPEUartSocketGetDataAccessRelease();
        return;
    }
    //从顶部弹出一个元素
    PE_UART_CMD_WAIT_PACK* waitPackPtr = (PE_UART_CMD_WAIT_PACK*)ADT_CircularPtrListPopTop(peUartInitSet.listSendCmdWaitAckPtr);
    //释放该元素的内存
    if(waitPackPtr == NULL)
    {
        //释放访问量
        ProtocolPEUartSocketGetDataAccessRelease();
        return;
    }
    if((waitPackPtr->cmdParamBufferPtr != NULL)&&(waitPackPtr->cmdParamBufferLength != 0))
    {
        //释放内存
        ProtocolPEUartSocketMemFree(waitPackPtr->cmdParamBufferPtr);
    }
    //释放内存
    ProtocolPEUartSocketMemFree(waitPackPtr);
    //释放访问量
    ProtocolPEUartSocketGetDataAccessRelease();
    return;
}


