#ifndef __IPC_BASE_H_
#define __IPC_BASE_H_
#include "IPC_Macro.h"
#include "IPC_DataType.h"

/*************************************************************************************************************/
//处理IPC指令的实现代码
typedef void (*UserIpcCmdProcessFunc)(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);

//IPC指令处理单元
typedef struct USER_IPC_CMD_PROC_UNIT
{
    uint32_t commandCode;//指令码
    uint32_t exceptParamLengthMin;//预期参数最小长度
    UserIpcCmdProcessFunc ipcCmdProcFuncPtr;//处理函数
}USER_IPC_CMD_PROC_UNIT;

/*************************************************************************************************************/
//处理IPC上报消息的实现代码
typedef void (*UserIpcUploadProcessFunc)(IPC_UPLOAD_DATA* ipcUploadDataPtr);
//IPC上报消息处理单元
typedef struct USER_IPC_UPLOAD_PROC_UNIT
{
    uint32_t uploadId;//上报ID
    uint32_t exceptParamLengthMin;//预期参数最小长度
    UserIpcUploadProcessFunc ipcUploadProcFuncPtr;//上报ID处理函数
}USER_IPC_UPLOAD_PROC_UNIT;

//设置指令包IPC默认值
void IPC_BaseSetCmdPackDefault(IPC_CMD_BASE_DATA* cmdBaseDataPtr);

//发送上传包
ERROR_SUB IPC_BaseSendUploadPack(TX_QUEUE* queuePtr,uint32_t uploadID,uint8_t* bufferPtr,uint16_t bufferLength);

//初始化通道数据
void IPC_BaseInitChannelData(IPC_CHANNEL_DATA* ipcChannelDataPtr,uint8_t* channelNameStr,TX_QUEUE* targetQueuePtr,
                                uint16_t dataBufferLength);

//通道清空,保证下一个指令可以正确传输
ERROR_SUB IPC_BaseChannelClearChannel(IPC_CHANNEL_DATA* channelDataPtr,uint32_t commandCode);

//检查通道状态是否存在上一个数据未完成或者完成后未读取
PROT_CH_STA IPC_BaseCheckCmdChannelState(IPC_CHANNEL_DATA* channelDataPtr,uint32_t commandCode);

//IPC指令发送并等待ACK返回
ERROR_SUB IPC_BaseSendCmdWhileAck(IPC_CHANNEL_DATA* channelDataPtr,IPC_CMD_BASE_DATA* cmdBaseDataPtr);

//IPC指令发送并等待结果返回
ERROR_SUB IPC_BaseSendCmdWhileReturn(IPC_CHANNEL_DATA* channelDataPtr,IPC_CMD_BASE_DATA* cmdBaseDataPtr,SYS_RESULT_PACK* resultPackPtr);

//IPC查询指令状态
ERROR_SUB IPC_BaseQueryCmdState(IPC_CHANNEL_DATA* channelDataPtr,uint32_t commandCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);

//IPC等待结果返回
ERROR_SUB IPC_BaseWaitCmdReturn(IPC_CHANNEL_DATA* channelDataPtr,uint32_t commandCode,SYS_RESULT_PACK* resultPackPtr);

//IPC查询上一次发出的指令状态
ERROR_SUB IPC_BaseQueryLastCmdState(IPC_CHANNEL_DATA* channelDataPtr,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultBasePackPtr);

//IPC等待上一次发出的指令结果返回
ERROR_SUB IPC_BaseWaitLastCmdReturn(IPC_CHANNEL_DATA* channelDataPtr,SYS_RESULT_PACK* resultBasePackPtr);

#endif



