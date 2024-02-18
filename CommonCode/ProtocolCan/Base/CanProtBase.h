/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-20 21:09:42 +0800
************************************************************************************************/ 
#ifndef __CAN_PROT_BASE_H_
#define __CAN_PROT_BASE_H_
#include "CanProtDataType.h"
#include "CanProtUtil.h"

//通过ID组合计算最终CAN ID
uint32_t CanProtBaseCalcID(CAN_PROT_ID* idPtr);

//将接收的ID转换为协议ID
void CanProtBasePackIdAnaly(uint32_t canID,CAN_PROT_ID* idPtr);

//数据包发送
ERROR_SUB CanProtBaseSendPack(PORT_CAN canPort,CAN_PROT_PACK* packPtr);


/**************************************************************CAN1 PORT************************************************************************/
//CAN1_PORT发送指令包,仅发送不做数据存储
ERROR_SUB Can1ProtSendCmdOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                            uint8_t retryCount,uint32_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength);
//CAN1_PORT发送指令包,仅发送不做数据存储
ERROR_SUB Can1ProtSendCmdOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                                uint8_t* paramBuffer,uint16_t paramLength);
//CAN1_PORT发送指令ACK包
ERROR_SUB Can1ProtSendAckCmd(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                        uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd);
//CAN1_PORT发送指令ACK包
ERROR_SUB Can1ProtSendAckCmdSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                        PROT_ACK ackCmd);
//CAN1_PORT发送结果包,仅发送不做数据存储
ERROR_SUB Can1ProtSendResultOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                    uint8_t retryCount,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,
                                    ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen);
//CAN1_PORT发送结果包,仅发送不做数据存储
ERROR_SUB Can1ProtSendResultOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                        PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                        uint8_t* resultParamBuffer,uint16_t resultBufferLen);
//CAN1_PORT发送结果ACK包
ERROR_SUB Can1ProtSendAckResult(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                    uint8_t retryCount,uint32_t cmdVal);                                                
//CAN1_PORT发送结果ACK包
ERROR_SUB Can1ProtSendAckResultSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal);
//CAN1_PORT发送数据上传包,仅发送不做数据存储
ERROR_SUB Can1ProtSendUploadOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                    uint8_t* uploadDataBuffer,uint16_t uploadDataLen);
//CAN1_PORT发送数据上传包,仅发送不做数据存储
ERROR_SUB Can1ProtSendUploadOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                        uint8_t* uploadDataBuffer,uint16_t uploadDataLen);
//CAN1_PORT发送数据上传ACK包
ERROR_SUB Can1ProtSendAckUpload(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                    uint32_t uploadDataID);
//CAN1_PORT发送数据上传ACK包
ERROR_SUB Can1ProtSendAckUploadSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID);


/**************************************************************CAN2 PORT************************************************************************/
//CAN_PORT2发送指令包,仅发送不做数据存储
ERROR_SUB Can2ProtSendCmdOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                            uint8_t retryCount,uint32_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength);
//CAN_PORT2发送指令包,仅发送不做数据存储
ERROR_SUB Can2ProtSendCmdOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                            uint8_t* paramBuffer,uint16_t paramLength);
//CAN_PORT2发送指令ACK包
ERROR_SUB Can2ProtSendAckCmd(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                        uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd);
//CAN_PORT2发送指令ACK包,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendAckCmdSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                                PROT_ACK ackCmd);
//CAN_PORT2发送结果包,仅发送不做数据存储
ERROR_SUB Can2ProtSendResultOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                            uint8_t retryCount,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,
                                            ERROR_LEVEL errorLevel,ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen);
//CAN_PORT2发送结果包,仅发送不做数据存储,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendResultOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                                    PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                                    uint8_t* resultParamBuffer,uint16_t resultBufferLen);
//CAN_PORT2发送结果ACK包
ERROR_SUB Can2ProtSendAckResult(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                           uint8_t retryCount,uint32_t cmdVal);
//CAN_PORT2发送结果ACK包,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendAckResultSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal);
//CAN_PORT2发送数据上传包,仅发送不做数据存储
ERROR_SUB Can2ProtSendUploadOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                            uint8_t* uploadDataBuffer,uint16_t uploadDataLen);
//CAN_PORT2发送数据上传包,仅发送不做数据存储,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendUploadOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                                    uint8_t* uploadDataBuffer,uint16_t uploadDataLen);
//CAN_PORT2发送数据上传ACK包
ERROR_SUB Can2ProtSendAckUpload(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID);
//CAN_PORT2发送数据上传ACK包,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendAckUploadSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID);



#endif

































