/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-03 14:52:37 +0800
************************************************************************************************/ 
#ifndef __NET_PROT_MAIN_H_
#define __NET_PROT_MAIN_H_
#include "NetProtBase.h"

//初始化SOCKET
void NetProtInit(uint8_t selfModuleNo,uint8_t selfBoardID);

//检查SOCKET是否已经初始化
FlagStatus NetProtInitQuery(void);

//SOCKET数据包接收
void NetProtPackRecv(NET_RECV_PACK* netRecvPackPtr,uint32_t currentTickMs);

//NET_PROT ACK发送
void NetProtAckSend(NET_PROT_ACK_PACK* ackMsgPtr);

//SOCKET内部周期性数据处理
void NetProtRunPeriod(uint32_t currentTickMs);

//NET在接收数据时候发送ACK
void NetProtSendAckInRecv(NET_PROT_ACK_TYPE ackType,uint8_t targetModule,uint8_t targetBoardID,
                            uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd);

//初始化发送指令
void NetProtSetDefaultCmdSend(NET_PROT_CMD_SEND* netSendPtr);

//获取指定指令的配置讯息
NET_CMD_CONFIG* NetProtGetCmdConfig(uint32_t cmdCode,NET_CMD_CONFIG* configArray,uint16_t arrayLen);

//创建发送指令
NET_PROT_CMD_SEND* NetProtCreateCmdSend(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdCode,NET_CMD_CONFIG* netCmdConfigPtr);

//销毁发送指令
void NetProtDestroyCmdSend(NET_PROT_CMD_SEND** netCmdSendPtrPtr);

//检查通道状态
PROT_CH_STA NetProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal);

//发送结果,带重发功能
void NetProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,
                        ERROR_LEVEL errorLevel,ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen,
                        SYS_RESULT_PACK* resultPackPtr); 

//发送上传,带重发功能
void NetProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                        uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr);

//发送指令,基础,只要发出去
void NetProtSendCmdNoWait(NET_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);

//发送指令,并等待ACK
void NetProtSendCmdWaitAck(NET_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr);

//等待结果
void NetProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen);

//发送指令并等待结果返回
void NetProtSendCmdWaitResult(NET_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                uint16_t* resultBufLen);

//清理指定通道的上一次指令
void NetProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr);

#endif











