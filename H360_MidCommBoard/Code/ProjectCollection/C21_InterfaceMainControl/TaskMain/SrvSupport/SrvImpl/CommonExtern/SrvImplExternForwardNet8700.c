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
#include "SrvImplBase.h"
#include "SrvTaskHeader.h"


//AckCmd数据转发,覆盖基础实现的项目实现
void SrvImplForwardSocketNetMsgAckCmd(NET_PROT_ACK_CMD_PACK_RECV* ackCmdPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
    uint32_t cmdCode = ackCmdPackPtr->ackCmdCode;
    uint8_t channel = 0X00;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_NET == CONFIG_FUNC_ENABLE)
    SystemPrintf("NetMsgAckCmd Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,AckCode: %d,RetryCount: %d\r\n",
                    ackCmdPackPtr->srcModuleNo,ackCmdPackPtr->srcBoardID,ackCmdPackPtr->targetModule,
                    ackCmdPackPtr->targetBoardID,ackCmdPackPtr->ackCode,ackCmdPackPtr->retryCount);
#endif
    if(cmdCode < 0X80000000)
    {
        SystemPrintf("Forward Cmd Code Must More Then 0X80000000\r\n");
        return;
    }
    cmdCode -= 0X80000000;
    channel = (uint8_t)(cmdCode >> 24);
    //去除头部
    cmdCode &= 0X00FFFFFF;
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    ackCmdPackPtr->targetModule = SYSTEM_MODULE_NO_ACT;
#endif
    //计算SOCKET
    PORT_CAN  portCan = CanProtPortCalcPort(ackCmdPackPtr->targetModule,ackCmdPackPtr->targetBoardID);
    if(portCan == PORT_CAN1)
    {
        //转发
        errorSub = Can1ProtSendAckCmd(ackCmdPackPtr->srcModuleNo,ackCmdPackPtr->srcBoardID,ackCmdPackPtr->targetModule,
                                                ackCmdPackPtr->targetBoardID,channel,ackCmdPackPtr->retryCount,cmdCode,ackCmdPackPtr->ackCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can1ProtSendAckCmd Failed: 0X%08X \r\n",errorSub);
        }
    }
    else 
    {
        //转发
        errorSub = Can2ProtSendAckCmd(ackCmdPackPtr->srcModuleNo,ackCmdPackPtr->srcBoardID,ackCmdPackPtr->targetModule,
                                                ackCmdPackPtr->targetBoardID,channel,ackCmdPackPtr->retryCount,cmdCode,ackCmdPackPtr->ackCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can2ProtSendAckCmd Failed: 0X%08X \r\n",errorSub);
        }
    }
}

//AckResult数据转发,覆盖基础实现的项目实现
void SrvImplForwardSocketNetMsgAckResult(NET_PROT_ACK_RESULT_PACK_RECV* ackResultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
    uint32_t cmdCode = ackResultPackPtr->ackCmdCode;
    uint8_t channel = 0X00;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_NET == CONFIG_FUNC_ENABLE)
    SystemPrintf("NetMsgAckResult Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,AckCode: 0X%08X,RetryCount: %d\r\n",
                    ackResultPackPtr->srcModuleNo,ackResultPackPtr->srcBoardID,ackResultPackPtr->targetModule,
                    ackResultPackPtr->targetBoardID,ackResultPackPtr->ackCmdCode,ackResultPackPtr->retryCount);
#endif
    if(cmdCode < 0X80000000)
    {
        SystemPrintf("Forward Cmd Code Must More Then 0X80000000\r\n");
        return;
    }
    cmdCode -= 0X80000000;
    channel = (uint8_t)(cmdCode >> 24);
    //去除头部
    cmdCode &= 0X00FFFFFF;
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    ackResultPackPtr->targetModule = SYSTEM_MODULE_NO_ACT;
#endif
    //计算SOCKET
    PORT_CAN  portCan = CanProtPortCalcPort(ackResultPackPtr->targetModule,ackResultPackPtr->targetBoardID);
    if(portCan == PORT_CAN1)
    {
        //转发
        errorSub = Can1ProtSendAckResult(ackResultPackPtr->srcModuleNo,ackResultPackPtr->srcBoardID,ackResultPackPtr->targetModule,
                                                ackResultPackPtr->targetBoardID,channel,ackResultPackPtr->retryCount,cmdCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can1ProtSendAckResult Failed: 0X%08X \r\n",errorSub);
        }
    }
    else
    {
        //转发
        errorSub = Can2ProtSendAckResult(ackResultPackPtr->srcModuleNo,ackResultPackPtr->srcBoardID,ackResultPackPtr->targetModule,
                                                ackResultPackPtr->targetBoardID,channel,ackResultPackPtr->retryCount,cmdCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can2ProtSendAckResult Failed: 0X%08X \r\n",errorSub);
        }
    }
    
}

//AckUpload数据转发,覆盖基础实现的项目实现
void SrvImplForwardSocketNetMsgAckUpload(NET_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_NET == CONFIG_FUNC_ENABLE)
    SystemPrintf("NetMsgAckUpload Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,UploadID: 0X%08X,RetryCount: %d\r\n",
                    ackUploadPackPtr->srcModuleNo,ackUploadPackPtr->srcBoardID,ackUploadPackPtr->targetModule,
                    ackUploadPackPtr->targetBoardID,ackUploadPackPtr->uploadID,ackUploadPackPtr->retryCount);
#endif
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    ackUploadPackPtr->targetModule = SYSTEM_MODULE_NO_ACT;
#endif
    //计算SOCKET
    PORT_CAN  portCan = CanProtPortCalcPort(ackUploadPackPtr->targetModule,ackUploadPackPtr->targetBoardID);
    if(portCan == PORT_CAN1)
    {
        //转发
        errorSub = Can1ProtSendAckUpload(ackUploadPackPtr->srcModuleNo,ackUploadPackPtr->srcBoardID,ackUploadPackPtr->targetModule,
                                                ackUploadPackPtr->targetBoardID,ackUploadPackPtr->retryCount,ackUploadPackPtr->uploadID);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can1ProtSendAckUpload Failed: 0X%08X \r\n",errorSub);
        }
    }
    else
    {
        //转发
        errorSub = Can2ProtSendAckUpload(ackUploadPackPtr->srcModuleNo,ackUploadPackPtr->srcBoardID,ackUploadPackPtr->targetModule,
                                                ackUploadPackPtr->targetBoardID,ackUploadPackPtr->retryCount,ackUploadPackPtr->uploadID);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can2ProtSendAckUpload Failed: 0X%08X \r\n",errorSub);
        }
    }
}

//日志信息转发,覆盖基础实现的项目实现
void SrvImplForwardSocketNetMsgLogMsg(NET_PROT_LOG_PACK_RECV* logPackPtr)
{
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_NET == CONFIG_FUNC_ENABLE)
    SystemPrintf("NetMsgLogMsg Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,DataLength: %d\r\n",
                    logPackPtr->srcModuleNo,logPackPtr->srcBoardID,logPackPtr->targetModule,
                    logPackPtr->targetBoardID,logPackPtr->logBodyLength);
    if((logPackPtr->logBodyLength != 0)&&(logPackPtr->logBodyBufferPtr != NULL))
    {
        SystemSendBuffer(logPackPtr->logBodyBufferPtr,logPackPtr->logBodyLength);
    }
#endif
}

//指令信息转发,覆盖基础实现的项目实现
void SrvImplForwardSocketNetMsgCmdMsg(NET_PROT_CMD_PACK_RECV* cmdPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
    uint32_t cmdCode = cmdPackPtr->cmdCode;
    uint8_t channel = 0X00;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_NET == CONFIG_FUNC_ENABLE)
    SystemPrintf("NetMsgCmdMsg Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,cmdCode:  0X%08X,RetryCount: %d,dataBodyLength: %d\r\n",
                    cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->targetModule,cmdPackPtr->targetBoardID,cmdPackPtr->cmdCode,
                    cmdPackPtr->retryCount,cmdPackPtr->dataBodyLength);
#endif
    if(cmdCode < 0X80000000)
    {
        SystemPrintf("Forward Cmd Code Must More Then 0X80000000\r\n");
        return;
    }
    cmdCode -= 0X80000000;
    channel = (uint8_t)(cmdCode >> 24);
    //去除头部
    cmdCode &= 0X00FFFFFF;
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    cmdPackPtr->targetModule = SYSTEM_MODULE_NO_ACT;
#endif
    //计算SOCKET
    PORT_CAN  portCan = CanProtPortCalcPort(cmdPackPtr->targetModule,cmdPackPtr->targetBoardID);
    if(portCan == PORT_CAN1)
    {
        //转发
        errorSub = Can1ProtSendCmdOnly(cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->targetModule,
                                                cmdPackPtr->targetBoardID,channel,cmdPackPtr->retryCount,cmdCode,cmdPackPtr->dataBodyBufferPtr,
                                                cmdPackPtr->dataBodyLength);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can1ProtSendCmdOnly Failed: 0X%08X \r\n",errorSub);
        }
    }
    else
    {
        //转发
        errorSub = Can2ProtSendCmdOnly(cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->targetModule,
                                                cmdPackPtr->targetBoardID,channel,cmdPackPtr->retryCount,cmdCode,cmdPackPtr->dataBodyBufferPtr,
                                                cmdPackPtr->dataBodyLength);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can2ProtSendCmdOnly Failed: 0X%08X \r\n",errorSub);
        }
    }
}

//结果信息转发,覆盖基础实现的项目实现
void SrvImplForwardSocketNetMsgResultMsg(NET_PROT_RESULT_PACK_RECV* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
    uint32_t cmdCode = resultPackPtr->cmdCode;
    uint8_t channel = 0X00;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_NET == CONFIG_FUNC_ENABLE)
    SystemPrintf("NetMsgResultMsg Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Cmd:  0X%08X,RetryCount: %d,resultParamBufferLength: %d\r\n",
                    resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->targetModule,resultPackPtr->targetBoardID,
                    resultPackPtr->cmdCode,resultPackPtr->retryCount,resultPackPtr->resultParamBufferLength);
    if((resultPackPtr->resultCode != PROT_RESULT_SUCCESS)||(resultPackPtr->errorMainCode != 0x0000)||
        (resultPackPtr->errorSubCode != 0X00000000))
    {
        SystemPrintf("NetMsgResultMsg Forward,ResultCode: %d,ErrorMain: 0X%04X, ErrorLevel: %d, ErrorSub: 0X%08X\r\n",
                    resultPackPtr->resultCode,resultPackPtr->errorMainCode,resultPackPtr->errorLevelCode,
                    resultPackPtr->errorSubCode);
    }
#endif
    if(cmdCode < 0X80000000)
    {
        SystemPrintf("Forward Cmd Code Must More Then 0X80000000\r\n");
        return;
    }
    cmdCode -= 0X80000000;
    channel = (uint8_t)(cmdCode >> 24);
    //去除头部
    cmdCode &= 0X00FFFFFF;
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    resultPackPtr->targetModule = SYSTEM_MODULE_NO_ACT;
#endif
    //计算SOCKET
    PORT_CAN  portCan = CanProtPortCalcPort(resultPackPtr->targetModule,resultPackPtr->targetBoardID);
    if(portCan == PORT_CAN1)
    {
        //转发
        errorSub = Can1ProtSendResultOnly(resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->targetModule,
                                                resultPackPtr->targetBoardID,channel,resultPackPtr->retryCount,cmdCode,resultPackPtr->resultCode,
                                                resultPackPtr->errorMainCode,resultPackPtr->errorLevelCode,resultPackPtr->errorSubCode,
                                                resultPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferLength);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can1ProtSendResultOnly Failed: 0X%08X \r\n",errorSub);
        }
    }
    else
    {
        //转发
        errorSub = Can2ProtSendResultOnly(resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->targetModule,
                                                resultPackPtr->targetBoardID,channel,resultPackPtr->retryCount,cmdCode,resultPackPtr->resultCode,
                                                resultPackPtr->errorMainCode,resultPackPtr->errorLevelCode,resultPackPtr->errorSubCode,
                                                resultPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferLength);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can2ProtSendResultOnly Failed: 0X%08X \r\n",errorSub);
        }
    }
}

//主动上传信息转发,覆盖基础实现的项目实现
void SrvImplForwardSocketNetMsgUploadMsg(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_NET == CONFIG_FUNC_ENABLE)
    SystemPrintf("NetMsgUploadMsg Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,RetryCount: %d,UploadID: 0X%08X,paramBufferLength: %d\r\n",
                    uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,uploadPackPtr->targetModule,uploadPackPtr->targetBoardID,
                    uploadPackPtr->retryCount,uploadPackPtr->uploadID,uploadPackPtr->paramBufferLength);
#endif
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    uploadPackPtr->targetModule = SYSTEM_MODULE_NO_ACT;
#endif
    //计算SOCKET
    PORT_CAN  portCan = CanProtPortCalcPort(uploadPackPtr->targetModule,uploadPackPtr->targetBoardID);
    if(portCan == PORT_CAN1)
    {
        //转发
        errorSub = Can1ProtSendUploadOnly(uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,uploadPackPtr->targetModule,
                                                uploadPackPtr->targetBoardID,uploadPackPtr->retryCount,uploadPackPtr->uploadID,
                                                uploadPackPtr->paramBufferPtr,uploadPackPtr->paramBufferLength);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can1ProtSendUploadOnly Failed: 0X%08X \r\n",errorSub);
        }
    }
    else
    {
        //转发
        errorSub = Can2ProtSendUploadOnly(uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,uploadPackPtr->targetModule,
                                                uploadPackPtr->targetBoardID,uploadPackPtr->retryCount,uploadPackPtr->uploadID,
                                                uploadPackPtr->paramBufferPtr,uploadPackPtr->paramBufferLength);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Can2ProtSendUploadOnly Failed: 0X%08X \r\n",errorSub);
        }
    }
}







