/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-07 13:09:00 +0800
************************************************************************************************/ 
#include "ProtocolC21NetBase.h"

//NET SOCKET初始化标记
extern PROTOCOL_C21_NET_INIT_SET c21NetInitSet;


//基础发送函数
ERROR_SUB ProtocolC21NetSocketSendDataBase(uint16_t cmdCode,uint8_t* paramBuffer,uint16_t paramLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t* sendDataBufferPtr = NULL;
    uint16_t sendBufferLength = 12+paramLength;
    uint16_t indexUtil = 0;
    //内存申请
    sendDataBufferPtr = ProtocolC21NetSocketMemMallocWhileSuccess(sendBufferLength);
    //内存参数设置
    sendDataBufferPtr[0] = PROTOCOL_C21_NET_FRAME_HEAD_LEADER_A;
    sendDataBufferPtr[1] = PROTOCOL_C21_NET_FRAME_HEAD_LEADER_B;
    //指令码设置
    sendDataBufferPtr[3] = (uint8_t)(cmdCode>>8);
    sendDataBufferPtr[2] = (uint8_t)(cmdCode>>0);
    //数据包长度
    sendDataBufferPtr[5] = (uint8_t)(paramLength>>8);
    sendDataBufferPtr[4] = (uint8_t)(paramLength>>0);
    //计算数据区域校验和
    uint32_t sumCodeDataBody = 0;
    for(indexUtil = 0; indexUtil < paramLength;indexUtil++)
    {
        sumCodeDataBody += paramBuffer[indexUtil];
    }
    //包头校验和
    sendDataBufferPtr[9] = (uint8_t)(sumCodeDataBody>>24);
    sendDataBufferPtr[8] = (uint8_t)(sumCodeDataBody>>16);
    sendDataBufferPtr[7] = (uint8_t)(sumCodeDataBody>>8);
    sendDataBufferPtr[6] = (uint8_t)(sumCodeDataBody>>0);
    //计算包头校验和
    uint16_t sumCodeCmdHeader = 0;
    for(indexUtil = 0; indexUtil < 10;indexUtil++)
    {
        sumCodeCmdHeader += sendDataBufferPtr[indexUtil];
    }
    sendDataBufferPtr[11] = (uint8_t)(sumCodeCmdHeader>>8);
    sendDataBufferPtr[10] = (uint8_t)(sumCodeCmdHeader>>0);
    //拷贝参数
    if(sendDataBufferPtr != NULL)
    {
        UserMemCopy(sendDataBufferPtr+12,paramBuffer,paramLength);
    }
    //申请互斥信号量
    ProtocolC21NetSocketGetMutexLock();
    //数据发送
    errorSub = ProtocolC21NetSocketSendFrame(sendBufferLength,sendDataBufferPtr);
    //释放互斥信号量
    ProtocolC21NetSocketReleaseMutexLock();
    //释放内存
    ProtocolC21NetSocketMemFree(sendDataBufferPtr);
    return errorSub;
}

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB ProtocolC21NetSendCmdOnly(uint16_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength)
{
    return ProtocolC21NetSocketSendDataBase(cmdVal,paramBuffer,paramLength);
}

//SOCKET发送指令ACK包
ERROR_SUB ProtocolC21NetSendAckCmd(C21_ACK_RESULT ackResult)
{
    if(RESET == ProtocolC21NetSocketGetConnectFlag())
    {
        //SOCKET未连接
        return ERROR_SUB_BOARD_C21_NET_SOCKET_DISCONNECT;
    }
    return ProtocolC21NetSocketSendDataBase(PROTOCOL_C21_NET_ACK_CMD,(uint8_t*)(&ackResult),1);
}




