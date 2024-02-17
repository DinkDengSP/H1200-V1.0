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
void SrvImplForwardSocketCanMsgAckCmd(PORT_CAN portCan,CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
    uint32_t cmdCode = 0;
    cmdCode += ackCmdPackPtr->singalChannel;
    cmdCode <<= 24;
    cmdCode += 0X80000000;
    cmdCode += ackCmdPackPtr->ackCmdVal;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_CAN == CONFIG_FUNC_ENABLE)
    SystemPrintf("CanMsgAckCmd Forward,portCan: %d,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,SingalChannel: %d,AckCode: 0X%08X,RetryCount: %d\r\n",
                    (uint8_t)portCan,ackCmdPackPtr->srcModuleNo,ackCmdPackPtr->srcBoardID,ackCmdPackPtr->targetModuleNo,
                    ackCmdPackPtr->targetBoardID,ackCmdPackPtr->singalChannel,ackCmdPackPtr->ackCode,
                    ackCmdPackPtr->retryCount);
#endif
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    ackCmdPackPtr->srcModuleNo = SELF_MODULE_NO;
#endif
    //将ACK CMD转发到网口
    errorSub = NetProtSendAckCmd(ackCmdPackPtr->srcModuleNo,ackCmdPackPtr->srcBoardID,ackCmdPackPtr->targetModuleNo,
                                            ackCmdPackPtr->targetBoardID,ackCmdPackPtr->retryCount,cmdCode,ackCmdPackPtr->ackCode);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendAckCmd Failed: 0X%08X \r\n",errorSub);
    }
}

//AckResult数据转发,覆盖基础实现的项目实现
void SrvImplForwardSocketCanMsgAckResult(PORT_CAN portCan,CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
    uint32_t cmdCode = 0;
    cmdCode += ackResultPackPtr->singalChannel;
    cmdCode <<= 24;
    cmdCode += 0X80000000;
    cmdCode += ackResultPackPtr->ackCmdVal;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_CAN == CONFIG_FUNC_ENABLE)
    SystemPrintf("CanMsgAckResult Forward,portCan: %d,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,SingalChannel: %d,AckCmd:  0X%08X,RetryCount: %d\r\n",
                    (uint8_t)portCan,ackResultPackPtr->srcModuleNo,ackResultPackPtr->srcBoardID,ackResultPackPtr->targetModuleNo,
                    ackResultPackPtr->targetBoardID,ackResultPackPtr->singalChannel,ackResultPackPtr->ackCmdVal,
                    ackResultPackPtr->retryCount);
#endif
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    ackResultPackPtr->srcModuleNo = SELF_MODULE_NO;
#endif
    //将ACK RESULT转发到网口
    errorSub = NetProtSendAckResult(ackResultPackPtr->srcModuleNo,ackResultPackPtr->srcBoardID,ackResultPackPtr->targetModuleNo,
                                            ackResultPackPtr->targetBoardID,ackResultPackPtr->retryCount,cmdCode);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendAckResult Failed: 0X%08X \r\n",errorSub);
    }
}

//AckUpload数据转发,覆盖基础实现的项目实现
void SrvImplForwardSocketCanMsgAckUpload(PORT_CAN portCan,CAN_PROT_ACK_UPLOAD_PACK* ackUploadPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_CAN == CONFIG_FUNC_ENABLE)
    SystemPrintf("CanMsgAckUpload Forward,portCan: %d,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,SingalChannel: %d,UploadID: 0X%08X,RetryCount: %d\r\n",
                    (uint8_t)portCan,ackUploadPackPtr->srcModuleNo,ackUploadPackPtr->srcBoardID,ackUploadPackPtr->targetModuleNo,
                    ackUploadPackPtr->targetBoardID,ackUploadPackPtr->singalChannel,ackUploadPackPtr->uploadID,
                    ackUploadPackPtr->retryCount);
#endif
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    ackUploadPackPtr->srcModuleNo = SELF_MODULE_NO;
#endif
    //将ACK Upload转发到网口
    errorSub = NetProtSendAckUpload(ackUploadPackPtr->srcModuleNo,ackUploadPackPtr->srcBoardID,ackUploadPackPtr->targetModuleNo,
                                            ackUploadPackPtr->targetBoardID,ackUploadPackPtr->retryCount,ackUploadPackPtr->uploadID);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendAckUpload Failed: 0X%08X \r\n",errorSub);
    }
}

//日志信息转发,覆盖基础实现的项目实现
void SrvImplForwardSocketCanMsgLogMsg(PORT_CAN portCan,CAN_PROT_LOG_PACK* logPackPtr)
{
    //日志信息打印
    if((logPackPtr->logDataBufferLength != 0)&&(logPackPtr->logDataBufferPtr != NULL))
    {
        SystemSendBufferOther(logPackPtr->srcModuleNo,logPackPtr->srcBoardID,logPackPtr->logDataBufferPtr,logPackPtr->logDataBufferLength);
    }
}

//指令信息转发,覆盖基础实现的项目实现
void SrvImplForwardSocketCanMsgCmdMsg(PORT_CAN portCan,CAN_PROT_CMD_PACK* cmdPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
    uint32_t cmdCode = 0;
    cmdCode += cmdPackPtr->singalChannel;
    cmdCode <<= 24;
    cmdCode += 0X80000000;
    cmdCode += cmdPackPtr->cmdValue;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_CAN == CONFIG_FUNC_ENABLE)
    SystemPrintf("CanMsgCmdMsg Forward,portCan: %d,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,SingalChannel: %d,Cmd:  0X%08X,RetryCount: %d,ParamLength: %d\r\n",
                    (uint8_t)portCan,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->targetModuleNo,
                    cmdPackPtr->targetBoardID,cmdPackPtr->singalChannel,cmdPackPtr->cmdValue,cmdPackPtr->retryCount,
                    cmdPackPtr->paramBufferLength);
#endif
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    cmdPackPtr->srcModuleNo = SELF_MODULE_NO;
#endif
    //将cmd转发到网口
    errorSub = NetProtSendCmdOnly(cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->targetModuleNo,cmdPackPtr->targetBoardID,
                                                cmdPackPtr->retryCount,cmdCode,cmdPackPtr->paramBufferPtr,cmdPackPtr->paramBufferLength);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendCmdOnly Failed: 0X%08X \r\n",errorSub);
    }
}

//结果信息转发,覆盖基础实现的项目实现
void SrvImplForwardSocketCanMsgResultMsg(PORT_CAN portCan,CAN_PROT_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
    uint32_t cmdCode = 0;
    cmdCode += resultPackPtr->singalChannel;
    cmdCode <<= 24;
    cmdCode += 0X80000000;
    cmdCode += resultPackPtr->cmdValue;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_CAN == CONFIG_FUNC_ENABLE)
    SystemPrintf("CanMsgResultMsg Forward,portCan: %d,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,SingalChannel: %d,Cmd:  0X%08X,RetryCount: %d,ParamLength: %d\r\n",
                    (uint8_t)portCan,resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->targetModuleNo,
                    resultPackPtr->targetBoardID,resultPackPtr->singalChannel,resultPackPtr->cmdValue,resultPackPtr->retryCount,
                    resultPackPtr->resultParamBufferLength);
    //结果异常的情况下打印结果信息
    if((resultPackPtr->resultCode != PROT_RESULT_SUCCESS)||(resultPackPtr->errorMainCode != 0x0000)||
        (resultPackPtr->errorSubCode != 0X00000000))
    {
        SystemPrintf("CanMsgResultMsg Forward,portCan: %d,ResultCode: %d,ErrorMain: 0X%04X, ErrorLevel: %d, ErrorSub: 0X%08X\r\n",
                    (uint8_t)portCan,resultPackPtr->resultCode,resultPackPtr->errorMainCode,resultPackPtr->errorLevelCode,
                    resultPackPtr->errorSubCode);
    }
#endif
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    resultPackPtr->srcModuleNo = SELF_MODULE_NO;
#endif
    //将Result转发到网口
    errorSub = NetProtSendResultOnly(resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->targetModuleNo,resultPackPtr->targetBoardID,
                                                resultPackPtr->retryCount,cmdCode,resultPackPtr->resultCode,resultPackPtr->errorMainCode,resultPackPtr->errorLevelCode,
                                                resultPackPtr->errorSubCode,resultPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferLength);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendResultOnly Failed: 0X%08X \r\n",errorSub);
    }
}

//主动上传信息转发,覆盖基础实现的项目实现
void SrvImplForwardSocketCanMsgUploadMsg(PORT_CAN portCan,CAN_PROT_UPLOAD_PACK* uploadPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_SOCKET_CAN == CONFIG_FUNC_ENABLE)
    SystemPrintf("CanMsgUploadMsg Forward,portCan: %d,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,SingalChannel: %d,RetryCount: %d,UploadID: 0X%08X,DataLength: %d\r\n",
                    (uint8_t)portCan,uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,uploadPackPtr->targetModuleNo,uploadPackPtr->targetBoardID,
                    uploadPackPtr->singalChannel,uploadPackPtr->retryCount,uploadPackPtr->uploadID,uploadPackPtr->uploadDataBufferLength);
#endif
    //模块号强制设置相同,必须在计算后面
#if(PROT_MODULE_NO_FORCE_SAME == CONFIG_FUNC_ENABLE)
    uploadPackPtr->srcModuleNo = SELF_MODULE_NO;
#endif
    //主动上传信息转发
    errorSub = NetProtSendUploadOnly(uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,uploadPackPtr->targetModuleNo,uploadPackPtr->targetBoardID,
                                                uploadPackPtr->retryCount,uploadPackPtr->uploadID,uploadPackPtr->uploadDataBufferPtr,uploadPackPtr->uploadDataBufferLength);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendUploadOnly Failed: 0X%08X \r\n",errorSub);
    }
}







