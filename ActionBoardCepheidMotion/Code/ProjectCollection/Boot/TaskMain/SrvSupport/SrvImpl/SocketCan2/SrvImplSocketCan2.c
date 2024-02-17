/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 16:15:10 +0800
************************************************************************************************/ 
#include "SrvImplSocketCan2.h"
#include "SrvTaskHeader.h"
#include "tx_queue.h"

//将CAN接收到的数据转发到处理线程
ErrorStatus Can2RecvDataDispatchToSrvTask(CAN_RECV_PACK* canPackRecvPtr)
{
    uint32_t txResult = TX_SUCCESS;
    if(queueSrvTaskSocketCan2.tx_queue_id != TX_QUEUE_ID)
    {
        //还没初始化
        return ERROR;
    }
    txResult = tx_queue_send(&queueSrvTaskSocketCan2,(void*)(&canPackRecvPtr),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        MCU_RTT_SendStringInt("Can2RecvDataDispatchToSrvTask tx_queue_send Failed\r\n");
        return ERROR;
    }
    return SUCCESS;
}

//任务全局对象初始化
void SrvImplSocketCan2VarInit(void)
{
    asm("nop");
}

//CAN2接收数据头
static CAN_RECV_PACK can2RecvPackHeadLocalTemp;
static uint8_t can2RecvPackDataLocalTemp[MCU_CAN_SINGLE_PACK_DATA_MAX];
//CAN2接收数据处理
void SrvImplCan2SocketMsgProcess(CAN_RECV_PACK* canRecvPackPtr)
{
    //调用SOCKET2数据处理
    uint32_t currentTimeTick = tx_time_get();
    //转换为毫秒
    currentTimeTick = OS_TICK_TO_MS(currentTimeTick);
    Can2ProtPackRecv(canRecvPackPtr,currentTimeTick);
}

//任务接收消息处理
void SrvImplSocketCan2MsgProcess(void* taskMsgPtr)
{
    CAN_RECV_PACK* canRecvPackPtr = (CAN_RECV_PACK*)taskMsgPtr;
    CAN_PACK_SRC canPackSrc = canRecvPackPtr->packSrc;
    //识别接收对象并拷贝数据,然后处理
    if(canPackSrc == CAN_PACK_SRC_2)
    {
        //数据拷贝
        UserMemCopy((void*)&can2RecvPackHeadLocalTemp,(void*)canRecvPackPtr,DIM_STRUCT_LENGTH_BYTE(CAN_RECV_PACK));
        UserMemCopy(can2RecvPackDataLocalTemp,canRecvPackPtr->recvDataBufferPtr,canRecvPackPtr->recvDataLength);
        //数据关联
        can2RecvPackHeadLocalTemp.recvDataBufferPtr = can2RecvPackDataLocalTemp;
        //处理完成,将数据放回队列
        BoardCAN2_RecvDatRelease(canRecvPackPtr);
        //处理接收到的数据
        SrvImplCan2SocketMsgProcess(&can2RecvPackHeadLocalTemp);
    }
    else
    {
        SystemPrintf("E: %s,Recv Unsupport Can Frame\r\n",__func__);
    }
}

//ACK消息发送处理
void SrvImplSocketCan2AckSendMsgProcess(void* socketAckMsgSendPtr)
{
    CAN_PROT_ACK_PACK* can2ProtAckPackPtr = (CAN_PROT_ACK_PACK*)socketAckMsgSendPtr;
    Can2ProtAckSend(can2ProtAckPackPtr);
    can2ProtAckPackPtr = NULL;
}

//任务状态机初始化
void SrvImplSocketCan2FSM_Init(void)
{
    asm("nop");
}


//任务状态机周期运行
void SrvImplSocketCan2FSM_RunPeriod(void)
{
    uint32_t currentTimeTick = tx_time_get();
    //转换为毫秒
    currentTimeTick = OS_TICK_TO_MS(currentTimeTick);
    //调用SOCKET2周期运转
    Can2ProtRunPeriod(currentTimeTick);
}
























