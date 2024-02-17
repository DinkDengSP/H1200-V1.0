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
#include "SrvImplForwardSocketUart.h"
#include "CommonSrvTaskHeader.h"


//AckCmd数据转发,基础实现是串口直接转网口,如果存在串口设备需要转换,那么自己重写实现
//这是一个弱定义实现,可以被外部相同定义的函数覆盖
__weak void SrvImplForwardSocketUartMsgAckCmd(UART_PROT_ACK_CMD_PACK_RECV* ackCmdPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //透传指令转换
#if(LOG_SHOW_SWITCH_FORWARD_UART == CONFIG_FUNC_ENABLE)
    SystemPrintf("UartMsgAckCmd Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,AckCode: %d,RetryCount: %d\r\n",
                    ackCmdPackPtr->srcModuleNo,ackCmdPackPtr->srcBoardID,ackCmdPackPtr->targetModule,
                    ackCmdPackPtr->targetBoardID,ackCmdPackPtr->ackCode,ackCmdPackPtr->retryCount);
#endif
    //将ACK CMD转发到网口
    errorSub = NetProtSendAckCmd(ackCmdPackPtr->srcModuleNo,ackCmdPackPtr->srcBoardID,ackCmdPackPtr->targetModule,
                                            ackCmdPackPtr->targetBoardID,ackCmdPackPtr->retryCount,ackCmdPackPtr->ackCmdCode,
                                            ackCmdPackPtr->ackCode);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendAckCmd Failed: 0X%08X \r\n",errorSub);
    }
}

//AckResult数据转发,基础实现是串口直接转网口,如果存在串口设备需要转换,那么自己重写实现
//这是一个弱定义实现,可以被外部相同定义的函数覆盖
__weak void SrvImplForwardSocketUartMsgAckResult(UART_PROT_ACK_RESULT_PACK_RECV* ackResultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_UART == CONFIG_FUNC_ENABLE)
    SystemPrintf("UartMsgAckResult Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,AckCode: 0X%08X,RetryCount: %d\r\n",
                    ackResultPackPtr->srcModuleNo,ackResultPackPtr->srcBoardID,ackResultPackPtr->targetModule,
                    ackResultPackPtr->targetBoardID,ackResultPackPtr->ackCmdCode,ackResultPackPtr->retryCount);
#endif
    //将ACK RESULT转发到网口
    errorSub = NetProtSendAckResult(ackResultPackPtr->srcModuleNo,ackResultPackPtr->srcBoardID,ackResultPackPtr->targetModule,
                                            ackResultPackPtr->targetBoardID,ackResultPackPtr->retryCount,ackResultPackPtr->ackCmdCode);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendAckResult Failed: 0X%08X \r\n",errorSub);
    }
    
}

//AckUpload数据转发,基础实现是串口直接转网口,如果存在串口设备需要转换,那么自己重写实现
//这是一个弱定义实现,可以被外部相同定义的函数覆盖
__weak void SrvImplForwardSocketUartMsgAckUpload(UART_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_UART == CONFIG_FUNC_ENABLE)
    SystemPrintf("UartMsgAckUpload Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,UploadID: 0X%08X,RetryCount: %d\r\n",
                    ackUploadPackPtr->srcModuleNo,ackUploadPackPtr->srcBoardID,ackUploadPackPtr->targetModule,
                    ackUploadPackPtr->targetBoardID,ackUploadPackPtr->uploadID,ackUploadPackPtr->retryCount);
#endif
    //将ACK Upload转发到网口
    errorSub = NetProtSendAckUpload(ackUploadPackPtr->srcModuleNo,ackUploadPackPtr->srcBoardID,ackUploadPackPtr->targetModule,
                                            ackUploadPackPtr->targetBoardID,ackUploadPackPtr->retryCount,ackUploadPackPtr->uploadID);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendAckUpload Failed: 0X%08X \r\n",errorSub);
    }
}

//日志信息转发,基础实现是串口直接转网口,如果存在串口设备需要转换,那么自己重写实现
//这是一个弱定义实现,可以被外部相同定义的函数覆盖
__weak void SrvImplForwardSocketUartMsgLogMsg(UART_PROT_LOG_PACK_RECV* logPackPtr)
{
    //日志信息打印
    if((logPackPtr->logBodyLength != 0)&&(logPackPtr->logBodyBufferPtr != NULL))
    {
        SystemSendBufferOther(logPackPtr->srcModuleNo,logPackPtr->srcBoardID,logPackPtr->logBodyBufferPtr,logPackPtr->logBodyLength);
    }
}

//指令信息转发,基础实现是串口直接转网口,如果存在串口设备需要转换,那么自己重写实现
//这是一个弱定义实现,可以被外部相同定义的函数覆盖
__weak void SrvImplForwardSocketUartMsgCmdMsg(UART_PROT_CMD_PACK_RECV* cmdPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_UART == CONFIG_FUNC_ENABLE)
    SystemPrintf("UartMsgCmdMsg Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,ackCmdPackPtr->ackCmdCode:  0X%08X,RetryCount: %d,dataBodyLength: %d\r\n",
                    cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->targetModule,cmdPackPtr->targetBoardID,cmdPackPtr->cmdCode,
                    cmdPackPtr->retryCount,cmdPackPtr->dataBodyLength);
#endif
    //将cmd转发到网口
    errorSub = NetProtSendCmdOnly(cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->targetModule,cmdPackPtr->targetBoardID,
                                                cmdPackPtr->retryCount,cmdPackPtr->cmdCode,cmdPackPtr->dataBodyBufferPtr,
                                                cmdPackPtr->dataBodyLength);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendCmdOnly Failed: 0X%08X \r\n",errorSub);
    }
}

//结果信息转发,基础实现是串口直接转网口,如果存在串口设备需要转换,那么自己重写实现
//这是一个弱定义实现,可以被外部相同定义的函数覆盖
__weak void SrvImplForwardSocketUartMsgResultMsg(UART_PROT_RESULT_PACK_RECV* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_UART == CONFIG_FUNC_ENABLE)
    SystemPrintf("UartMsgResultMsg Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Cmd:  0X%08X,RetryCount: %d,resultParamBufferLength: %d\r\n",
                    resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->targetModule,resultPackPtr->targetBoardID,
                    resultPackPtr->cmdCode,resultPackPtr->retryCount,resultPackPtr->resultParamBufferLength);
    SystemPrintf("UartMsgResultMsg Forward,ResultCode: %d,ErrorMain: 0X%04X, ErrorLevel: %d, ErrorSub: 0X%08X\r\n",
                    resultPackPtr->resultCode,resultPackPtr->errorMainCode,resultPackPtr->errorLevelCode,
                    resultPackPtr->errorSubCode);
#endif
    //将Result转发到网口
    errorSub = NetProtSendResultOnly(resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->targetModule,resultPackPtr->targetBoardID,
                                                resultPackPtr->retryCount,resultPackPtr->cmdCode,resultPackPtr->resultCode,resultPackPtr->errorMainCode,resultPackPtr->errorLevelCode,
                                                resultPackPtr->errorSubCode,resultPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferLength);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendResultOnly Failed: 0X%08X \r\n",errorSub);
    }
}

//主动上传信息转发,基础实现是串口直接转网口,如果存在串口设备需要转换,那么自己重写实现
//这是一个弱定义实现,可以被外部相同定义的函数覆盖
__weak void SrvImplForwardSocketUartMsgUploadMsg(UART_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
#if(LOG_SHOW_SWITCH_FORWARD_UART == CONFIG_FUNC_ENABLE)
    SystemPrintf("UartMsgUploadMsg Forward,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,RetryCount: %d,UploadID: 0X%08X,paramBufferLength: %d\r\n",
                    uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,uploadPackPtr->targetModule,uploadPackPtr->targetBoardID,
                    uploadPackPtr->retryCount,uploadPackPtr->uploadID,uploadPackPtr->paramBufferLength);
#endif
    //主动上传信息转发
    errorSub = NetProtSendUploadOnly(uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,uploadPackPtr->targetModule,uploadPackPtr->targetBoardID,
                                                uploadPackPtr->retryCount,uploadPackPtr->uploadID,uploadPackPtr->paramBufferPtr,
                                                uploadPackPtr->paramBufferLength);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,NetProtSendUploadOnly Failed: 0X%08X \r\n",errorSub);
    }
}







