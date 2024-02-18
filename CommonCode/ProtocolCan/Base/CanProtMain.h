/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-02 20:34:23 +0800
************************************************************************************************/ 
#ifndef __CAN_PROT_MAIN_H_
#define __CAN_PROT_MAIN_H_
#include "CanProtBase.h"

/***********************************************************CAN1_PROT************************************************************/
//初始化CAN1_PORT
void Can1ProtInit(uint8_t selfModuleNo,uint8_t selfBoardID);
//检查SOCKET是否已经初始化
FlagStatus Can1ProtInitQuery(void);
//CAN1_PORT数据包接收
void Can1ProtPackRecv(CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//CAN1_PROT ACK发送
void Can1ProtAckSend(CAN_PROT_ACK_PACK* ackMsgPtr);
//CAN1_PORT内部周期性数据处理
void Can1ProtRunPeriod(uint32_t currentTickMs);
//CAN1在接收数据时候发送ACK
void Can1ProtSendAckInRecv(CAN_PROT_ACK_TYPE ackType,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                            uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd);
//CAN1_PROT发送日志
void Can1ProtSendLog(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                            uint8_t retryCount,uint8_t* logBuffer,uint16_t logLength);
//CAN1_PROT发送日志,简易模式
void Can1ProtSendLogSample(uint8_t* logBuffer,uint16_t logLength);
//CAN1_PROT发送结果,带重发功能
void Can1ProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen,SYS_RESULT_PACK* resultPackPtr);
//CAN1_PROT发送上传,带重发功能
void Can1ProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr);
//查询指令的当前状态
ERROR_SUB Can1ProtQueryCmdState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,PROT_CMD_STA* cmdStage);
//检查通道状态
PROT_CH_STA Can1ProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal);
//CAN1_PROT发送指令,基础,只要发出去
void Can1ProtSendCmdNoWait(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);
//CAN1_PROT发送指令,并等待ACK
void Can1ProtSendCmdWaitAck(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);
//CAN1_PROT等待指令结果
void Can1ProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen);
//CAN1_PROT发送指令并等待结果返回
void Can1ProtSendCmdWaitResult(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                    uint16_t* resultBufLen);
//清理指定通道的上一次指令
void Can1ProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdCode,
                                SYS_RESULT_PACK* resultPackPtr);
//回传指令结果
void Can1ProtSendResultByRecv(CAN_PROT_CMD_PACK* canCmdRecvPtr,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen);
/***********************************************************CAN2_PROT************************************************************/
//初始化CAN2_PORT
void Can2ProtInit(uint8_t selfModuleNo,uint8_t selfBoardID);
//检查SOCKET是否已经初始化
FlagStatus Can2ProtInitQuery(void);
//CAN2_PORT数据包接收
void Can2ProtPackRecv(CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//CAN2_PROT ACK发送
void Can2ProtAckSend(CAN_PROT_ACK_PACK* ackMsgPtr);
//CAN2_PORT内部周期性数据处理
void Can2ProtRunPeriod(uint32_t currentTickMs);
//CAN2在接收数据时候发送ACK
void Can2ProtSendAckInRecv(CAN_PROT_ACK_TYPE ackType,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                            uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd);
//CAN2_PROT发送日志
void Can2ProtSendLog(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                            uint8_t retryCount,uint8_t* logBuffer,uint16_t logLength);
//CAN2_PROT发送日志,简易模式
void Can2ProtSendLogSample(uint8_t* logBuffer,uint16_t logLength);
//CAN2_PROT发送结果,带重发功能
void Can2ProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen,SYS_RESULT_PACK* resultPackPtr);
//CAN2_PROT发送上传,带重发功能
void Can2ProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr);
//查询指令的当前状态
ERROR_SUB Can2ProtQueryCmdState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,PROT_CMD_STA* cmdStage);
//检查通道状态
PROT_CH_STA Can2ProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal);
//CAN2_PROT发送指令,基础,只要发出去
void Can2ProtSendCmdNoWait(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);
//CAN2_PROT发送指令,并等待ACK
void Can2ProtSendCmdWaitAck(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);
//CAN2_PROT等待指令结果
void Can2ProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen);
//CAN2_PROT发送指令并等待结果返回
void Can2ProtSendCmdWaitResult(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                    uint16_t* resultBufLen);
//清理指定通道的上一次指令
void Can2ProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdCode,
                                SYS_RESULT_PACK* resultPackPtr);
//回传指令结果
void Can2ProtSendResultByRecv(CAN_PROT_CMD_PACK* canCmdRecvPtr,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen);
/***********************************************************CAN1_CAN2共用************************************************************/
//申请内存
uint8_t* CanProtMemMalloc(uint8_t targetModule,uint8_t targetBoardID,uint32_t byteCount);
//释放内存
void CanProtMemFree(uint8_t targetModule,uint8_t targetBoardID,uint8_t* memPtr);
//创建结果包
SYS_RESULT_PACK* CanProtCreateSysResultPack(uint8_t targetModule,uint8_t targetBoardID);
//释放结果包
void CanProtFreeSysResultPack(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* sysResultPackPtr,uint8_t* resultDataBufPtr,uint16_t resultBufLen);
//释放接收缓存
void CanProtFreeRecvDataBuf(uint8_t targetModule,uint8_t targetBoardID,uint8_t* resultDataBufPtr,uint16_t resultBufLen);
//获取指定指令的配置讯息
CAN_PROT_CMD_CONFIG* CanProtGetCmdConfig(uint32_t cmd,CAN_PROT_CMD_CONFIG* configArray,uint16_t arrayLen);
//创建发送指令
CAN_PROT_CMD_SEND* CanProtCreateCmdSend(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdCode,
                                                CAN_PROT_CMD_CONFIG* canCmdConfigPtr);
//销毁发送指令
void CanProtDestroyCmdSend(CAN_PROT_CMD_SEND** canCmdSendPtrPtr);

//CAN_PROT发送结果,带重发功能
void CanProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen,SYS_RESULT_PACK* resultPackPtr);
//CAN_PROT发送上传,带重发功能
void CanProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr);
//查询指令的当前状态
ERROR_SUB CanProtQueryCmdState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,PROT_CMD_STA* cmdStage);
//检查通道状态
PROT_CH_STA CanProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal);
//CAN_PROT发送指令,基础,只要发出去
void CanProtSendCmdNoWait(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);
//CAN_PROT发送指令,并等待ACK
void CanProtSendCmdWaitAck(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);
//CAN_PROT等待指令结果
void CanProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen);
//CAN_PROT发送指令并等待结果返回
void CanProtSendCmdWaitResult(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                    uint16_t* resultBufLen);
//清理指定通道的上一次指令
void CanProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdCode,
                                SYS_RESULT_PACK* resultPackPtr);



#endif

































