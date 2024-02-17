/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 16:23:34 +0800
************************************************************************************************/ 
#include "SrvImplUart6Recv.h"
#include "CommonSrvTaskHeader.h"
#include "tx_queue.h"

//消息转发
ErrorStatus Uart6RecvMsgDispatch(UART_RECV_PACK* uartPackRecvPtr)
{
    uint32_t txResult = TX_SUCCESS;
    //消息队列没有初始化
    if(queueSrvTaskUart6Recv.tx_queue_id != TX_QUEUE_ID)
    {
        return ERROR;
    }
    //转发
    txResult = tx_queue_send(&queueSrvTaskUart6Recv,(void*)(&uartPackRecvPtr),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        MCU_RTT_SendStringInt("queueSrvTaskUart6Recv tx_queue_send Failed\r\n");
        return ERROR;
    }
    return SUCCESS;
}

//任务全局对象初始化
void SrvImplUart6RecvVarInit(void)
{

}


//任务接收消息处理
void SrvImplUart6RecvMsgProcess(void* taskMsgPtr)
{
    UART_RECV_PACK* uartPackRecvPtr = (UART_RECV_PACK*)taskMsgPtr;
    if(uartPackRecvPtr->packSrc == UART_PACK_SRC_6)
    {
        //接收处理
        BoardEC20_ProcRecvBuf(uartPackRecvPtr->recvDataBufferPtr,uartPackRecvPtr->recvDataLength);
        //打印接收数据
        MCU_LogSendBuffer(uartPackRecvPtr->recvDataBufferPtr,uartPackRecvPtr->recvDataLength);
        //探针接口
#if(BOARD_EC20_PROBE_ENABLE == CONFIG_FUNC_ENABLE)
        SystemProbeSendBuffer(uartPackRecvPtr->recvDataBufferPtr,uartPackRecvPtr->recvDataLength);
#endif
        //处理完成,释放
        MCU_UART6_EC20_RecvDatRelease(uartPackRecvPtr);
    }
    
}


//任务状态机初始化
void SrvImplUart6RecvFSM_Init(void)
{

}

//任务状态机周期运行
void SrvImplUart6RecvFSM_RunPeriod(void)
{

}















