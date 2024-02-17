/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-18 10:33:47 +0800
************************************************************************************************/ 
#ifndef __UART_PROT_BASE_H_
#define __UART_PROT_BASE_H_
#include "UartProtDataType.h"
#include "UartProtUtil.h"

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB UartProtSendCmdOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength);
//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB UartProtSendCmdOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,uint8_t* paramBuffer,
                                                uint16_t paramLength);
//SOCKET发送指令ACK包
ERROR_SUB UartProtSendAckCmd(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,
                                            PROT_ACK ackCmd);
//SOCKET发送指令ACK包
ERROR_SUB UartProtSendAckCmdSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd);
//SOCKET发送结果包,仅发送不做数据存储
ERROR_SUB UartProtSendResultOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,
                                            uint8_t retryCount,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,
                                            ERROR_LEVEL errorLevel,ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen);
//SOCKET发送结果包,仅发送不做数据存储
ERROR_SUB UartProtSendResultOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,
                                                    PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                                    uint8_t* resultParamBuffer,uint16_t resultBufferLen);
//SOCKET发送结果ACK包
ERROR_SUB UartProtSendAckResult(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,
                                           uint8_t retryCount,uint32_t cmdVal);
//SOCKET发送结果ACK包
ERROR_SUB UartProtSendAckResultSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal);
//SOCKET发送数据上传包,仅发送不做数据存储
ERROR_SUB UartProtSendUploadOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID,uint8_t* uploadDataBuffer,uint16_t uploadDataLen);
//SOCKET发送数据上传包,仅发送不做数据存储
ERROR_SUB UartProtSendUploadOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                                    uint8_t* uploadDataBuffer,uint16_t uploadDataLen);
//SOCKET发送数据上传ACK包
ERROR_SUB UartProtSendAckUpload(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID);
//SOCKET发送数据上传ACK包
ERROR_SUB UartProtSendAckUploadSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID);
//UART SOCKET发送日志
ERROR_SUB UartProtSendLog(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                        uint8_t* logBuffer,uint16_t logLength);
//UART SOCKET发送日志,简易模式
ERROR_SUB UartProtSendLogSample(uint8_t* logBuffer,uint16_t logLength);
                                        
#endif









