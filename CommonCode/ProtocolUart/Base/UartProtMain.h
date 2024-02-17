/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-03 14:52:37 +0800
************************************************************************************************/ 
#ifndef __UART_PROT_MAIN_H_
#define __UART_PROT_MAIN_H_
#include "UartProtBase.h"

//初始化SOCKET
void UartProtInit(uint8_t selfModuleNo,uint8_t selfBoardID);

//检查SOCKET是否已经初始化
FlagStatus UartProtInitQuery(void);

//SOCKET数据包接收
void UartProtPackRecv(UART_RECV_PACK* uartRecvPackPtr,uint32_t currentTickMs);

//UART_PROT ACK发送
void UartProtAckSend(UART_PROT_ACK_PACK* ackMsgPtr);

//SOCKET内部周期性数据处理
void UartProtRunPeriod(uint32_t currentTickMs);

//UART在接收数据时候发送ACK
void UartProtSendAckInRecv(UART_PROT_ACK_TYPE ackType,uint8_t targetModule,uint8_t targetBoardID,
                            uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd);

//初始化发送指令
void UartProtSetDefaultCmdSend(UART_PROT_CMD_SEND* uartSendPtr);

//获取指定指令的配置讯息
UART_CMD_CONFIG* UartProtGetCmdConfig(uint32_t cmdCode,UART_CMD_CONFIG* configArray,uint16_t arrayLen);

//创建发送指令
UART_PROT_CMD_SEND* UartProtCreateCmdSend(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdCode,UART_CMD_CONFIG* uartCmdConfigPtr);

//销毁发送指令
void UartProtDestroyCmdSend(UART_PROT_CMD_SEND** uartCmdSendPtrPtr);

//检查通道状态
PROT_CH_STA UartProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal);

//发送结果,带重发功能
void UartProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,
                        ERROR_LEVEL errorLevel,ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen,
                        SYS_RESULT_PACK* resultPackPtr); 

//发送上传,带重发功能
void UartProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                        uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr);

//发送指令,基础,只要发出去
void UartProtSendCmdNoWait(UART_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);

//发送指令,并等待ACK
void UartProtSendCmdWaitAck(UART_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);

//等待结果
void UartProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen);

//发送指令并等待结果返回
void UartProtSendCmdWaitResult(UART_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                uint16_t* resultBufLen);

//清理指定通道的上一次指令
void UartProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr);

#endif











