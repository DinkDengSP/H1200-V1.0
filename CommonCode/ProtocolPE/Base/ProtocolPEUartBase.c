/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-05 16:37:21 +0800
************************************************************************************************/ 
#include "ProtocolPEUartBase.h"

//UART SOCKET初始化标记
extern PROTOCOL_PE_UART_INIT_SET peUartInitSet;


//基础发送函数
ERROR_SUB ProtocolPEUartSocketSendDataBase(uint16_t cmdCode,uint8_t* paramBuffer,uint16_t paramLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t* sendDataBufferPtr = NULL;
    uint16_t sendBufferLength = 12+paramLength;
    uint16_t indexUtil = 0;
    //内存申请
    sendDataBufferPtr = ProtocolPEUartSocketMemMallocWhileSuccess(sendBufferLength);
    //内存参数设置
    sendDataBufferPtr[0] = PROTOCOL_PE_UART_FRAME_HEAD_LEADER_A;
    sendDataBufferPtr[1] = PROTOCOL_PE_UART_FRAME_HEAD_LEADER_B;
    //指令码设置
    sendDataBufferPtr[2] = (uint8_t)(cmdCode>>8);
    sendDataBufferPtr[3] = (uint8_t)(cmdCode>>0);
    //数据包长度
    sendDataBufferPtr[4] = (uint8_t)(paramLength>>8);
    sendDataBufferPtr[5] = (uint8_t)(paramLength>>0);
    //计算数据区域校验和
    uint32_t sumCodeDataBody = 0;
    for(indexUtil = 0; indexUtil < paramLength;indexUtil++)
    {
        sumCodeDataBody += paramBuffer[indexUtil];
    }
    //包头校验和
    sendDataBufferPtr[6] = (uint8_t)(sumCodeDataBody>>24);
    sendDataBufferPtr[7] = (uint8_t)(sumCodeDataBody>>16);
    sendDataBufferPtr[8] = (uint8_t)(sumCodeDataBody>>8);
    sendDataBufferPtr[9] = (uint8_t)(sumCodeDataBody>>0);
    //计算包头校验和
    uint16_t sumCodeCmdHeader = 0;
    for(indexUtil = 0; indexUtil < 10;indexUtil++)
    {
        sumCodeCmdHeader += sendDataBufferPtr[indexUtil];
    }
    sendDataBufferPtr[10] = (uint8_t)(sumCodeCmdHeader>>8);
    sendDataBufferPtr[11] = (uint8_t)(sumCodeCmdHeader>>0);
    //拷贝参数
    if(sendDataBufferPtr != NULL)
    {
        UserMemCopy(sendDataBufferPtr+12,paramBuffer,paramLength);
    }
    //申请互斥信号量
    ProtocolPEUartSocketGetMutexLock();
    //数据发送
    errorSub = ProtocolPEUartSocketSendFrame(sendBufferLength,sendDataBufferPtr);
    //释放互斥信号量
    ProtocolPEUartSocketReleaseMutexLock();
    //释放内存
    ProtocolPEUartSocketMemFree(sendDataBufferPtr);
    return errorSub;
}

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB ProtocolPEUartSendCmdOnly(uint16_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength)
{
    return ProtocolPEUartSocketSendDataBase(cmdVal,paramBuffer,paramLength);
}

//SOCKET发送指令ACK包
ERROR_SUB ProtocolPEUartSendAckCmd(void)
{
    return ProtocolPEUartSocketSendDataBase(PROTOCOL_PE_UART_ACK_CMD,NULL,0);
}




