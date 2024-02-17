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
#include "SrvImplSocketCan1.h"
#include "CommonSrvTaskHeader.h"
#include "tx_queue.h"

//将CAN接收到的数据转发到处理线程
ErrorStatus Can1RecvDataDispatchToSrvTask(CAN_RECV_PACK* canPackRecvPtr)
{
    uint32_t txResult = TX_SUCCESS;
    if(queueSrvTaskSocketCan1.tx_queue_id != TX_QUEUE_ID)
    {
        //还没初始化
        return ERROR;
    }
    txResult = tx_queue_send(&queueSrvTaskSocketCan1,(void*)(&canPackRecvPtr),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        MCU_RTT_SendStringInt("Can1RecvDataDispatchToSrvTask tx_queue_send Failed\r\n");
        return ERROR;
    }
    return SUCCESS;
}

//任务全局对象初始化
void SrvImplSocketCan1VarInit(void)
{

}

//CAN1接收数据头
static CAN_RECV_PACK can1RecvPackHeadLocalTemp;
static uint8_t can1RecvPackDataLocalTemp[CAN1_PROT_DATA_BUF_LEN];

//任务接收消息处理
void SrvImplSocketCan1MsgProcess(void* taskMsgPtr)
{
    //调用数据处理
    uint32_t currentTimeTick = tx_time_get();
    CAN_RECV_PACK* canRecvPackPtr = (CAN_RECV_PACK*)taskMsgPtr;
    CAN_PACK_SRC canPackSrc = canRecvPackPtr->packSrc;
    if(canPackSrc == CAN_PACK_SRC_1)
    {
        //数据拷贝
        UserMemCopy((void*)&can1RecvPackHeadLocalTemp,(void*)canRecvPackPtr,DIM_STRUCT_LENGTH_BYTE(CAN_RECV_PACK));
        UserMemCopy(can1RecvPackDataLocalTemp,canRecvPackPtr->recvDataBufferPtr,canRecvPackPtr->recvDataLength);
        //数据关联
        can1RecvPackHeadLocalTemp.recvDataBufferPtr = can1RecvPackDataLocalTemp;
        //处理完成,将数据放回队列
        BoardCAN1_RecvDatRelease(canRecvPackPtr);
        //转换为毫秒
        currentTimeTick = OS_TICK_TO_MS(currentTimeTick);
        Can1ProtPackRecv(&can1RecvPackHeadLocalTemp,currentTimeTick);
    }
    else
    {
        SystemPrintf("E: %s,Recv Unsupport Can Frame\r\n",__func__);
    }
}

//ACK消息发送处理
void SrvImplSocketCan1AckSendMsgProcess(void* socketAckMsgSendPtr)
{
    CAN_PROT_ACK_PACK* can1ProtAckPackPtr = (CAN_PROT_ACK_PACK*)socketAckMsgSendPtr;
    Can1ProtAckSend(can1ProtAckPackPtr);
    can1ProtAckPackPtr = NULL;
}

//任务状态机初始化
void SrvImplSocketCan1FSM_Init(void)
{
    
}


//任务状态机周期运行
void SrvImplSocketCan1FSM_RunPeriod(void)
{
    uint32_t currentTimeTick = tx_time_get();
    //转换为毫秒
    currentTimeTick = OS_TICK_TO_MS(currentTimeTick);
    //调用CAN1_PROT周期运转
    Can1ProtRunPeriod(currentTimeTick);
}







