/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 18:43:57 +0800
************************************************************************************************/ 
#include "ProtocolC21NetBase.h"

//PE NET SOCKET初始化标记
extern PROTOCOL_C21_NET_INIT_SET c21NetInitSet;
//获取数据访问权
extern void ProtocolC21NetSocketGetDataAccessLock(void);
//释放数据访问权
extern void ProtocolC21NetSocketGetDataAccessRelease(void);

//指令ACK处理
void ProtocolC21NetSocketPackRecvProcessAckCmd(void)
{
    uint32_t elementCount = 0;
    //获取访问量
    ProtocolC21NetSocketGetDataAccessLock();
    //检查等待链表是否为空,空直接返回
    elementCount = ADT_CircularPtrListGetNodeCount(c21NetInitSet.listSendCmdWaitAckPtr);
    if(elementCount == 0)
    {
        //释放访问量
        ProtocolC21NetSocketGetDataAccessRelease();
        return;
    }
    //从顶部弹出一个元素
    C21_NET_CMD_WAIT_PACK* waitPackPtr = (C21_NET_CMD_WAIT_PACK*)ADT_CircularPtrListPopTop(c21NetInitSet.listSendCmdWaitAckPtr);
    //释放该元素的内存
    if(waitPackPtr == NULL)
    {
        //释放访问量
        ProtocolC21NetSocketGetDataAccessRelease();
        return;
    }
    if((waitPackPtr->cmdParamBufferPtr != NULL)&&(waitPackPtr->cmdParamBufferLength != 0))
    {
        //释放内存
        ProtocolC21NetSocketMemFree(waitPackPtr->cmdParamBufferPtr);
    }
    //释放内存
    ProtocolC21NetSocketMemFree(waitPackPtr);
    //释放访问量
    ProtocolC21NetSocketGetDataAccessRelease();
    return;
}


