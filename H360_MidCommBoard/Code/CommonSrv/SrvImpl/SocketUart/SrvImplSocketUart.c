/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 15:42:15
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplSocketUart.h"

//将UART接收到的数据转发到处理线程
ErrorStatus UartRecvDataDispatchToSrvTask(UART_RECV_PACK* uartRecvPackPtr)
{
    uint32_t txResult = TX_SUCCESS;
    if(queueSrvTaskSocketUart.tx_queue_id != TX_QUEUE_ID)
    {
        //还没初始化
        return ERROR;
    }
    txResult = tx_queue_send(&queueSrvTaskSocketUart,(void*)(&uartRecvPackPtr),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        MCU_RTT_SendStringInt("UartRecvDataDispatchToSrvTask tx_queue_send Failed\r\n");
        return ERROR;
    }
    return SUCCESS;
}

//任务全局对象初始化
void SrvImplSocketUartVarInit(void)
{
    
}

//任务接收消息处理
void SrvImplSocketUartMsgProcess(void* taskMsgPtr)
{
    //调用数据处理
    uint32_t currentTimeTick = tx_time_get();
    UART_RECV_PACK* uartRecvPackPtr = (UART_RECV_PACK*)taskMsgPtr;
    UART_PACK_SRC uartPackSrc = uartRecvPackPtr->packSrc;
    if(uartPackSrc == UART_PACK_SRC_1)
    {
#if(PROT_UART_USER_LH_LOCAL == CONFIG_FUNC_ENABLE)
        //转换为毫秒
        currentTimeTick = OS_TICK_TO_MS(currentTimeTick);
        UartProtPackRecv(uartRecvPackPtr,currentTimeTick);
#else
        ProtocolPEUartSocketPackRecv(uartRecvPackPtr,currentTimeTick);
#endif
        //释放
        MCU_UART1_RecvDatRelease(uartRecvPackPtr);
    }
    else
    {
        SystemPrintf("E: %s,Recv Unsupport Can Frame\r\n",__func__);
    }
}

//ACK消息发送处理
void SrvImplSocketUartAckSendMsgProcess(void* socketAckMsgSendPtr)
{
#if(PROT_UART_USER_LH_LOCAL == CONFIG_FUNC_ENABLE)
    UART_PROT_ACK_PACK* uartProtAckPackPtr = (UART_PROT_ACK_PACK*)socketAckMsgSendPtr;
    UartProtAckSend(uartProtAckPackPtr);
    uartProtAckPackPtr = NULL;
#endif
}

//任务状态机初始化
void SrvImplSocketUartFSM_Init(void)
{
    
}


//任务状态机周期运行
void SrvImplSocketUartFSM_RunPeriod(void)
{
    uint32_t currentTimeTick = tx_time_get();
    //转换为毫秒
    currentTimeTick = OS_TICK_TO_MS(currentTimeTick);
#if(PROT_UART_USER_LH_LOCAL == CONFIG_FUNC_ENABLE)
    //调用UART_PROT周期运转
    UartProtRunPeriod(currentTimeTick);
#else
    ProtocolPEUartRunPeriod(currentTimeTick);
#endif
}








