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
#include "SrvImplTimer.h"
#include "SrvImplBaseTimer.h"
#include "SrvTaskHeader.h"


/****************************************网口指令消息处理函数**************************************************/
//占位函数
static void SrvMsgUartTimerCmd_PlaceHolder(UART_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//UART指令处理函数列表
static const USER_UART_CMD_PROC_UNIT uartCmdProcTimerUnitArray[] = 
{
    //顺序: 指令码  预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {(uint32_t)UART_MASTER_CMD_HOLD_PLACE         ,0  ,ENABLE ,SrvMsgUartTimerCmd_PlaceHolder   },
};

void SrvMsgUartTimerCmd(void* uartCmdRecvPtr)
{
    //转换指令码
    UART_PROT_CMD_PACK_RECV* cmdPackPtr = (UART_PROT_CMD_PACK_RECV*)uartCmdRecvPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv UartCmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //接收打印显示
    SystemPrintf("Timer Recv Uart Cmd: 0X%08X,DataLen: %d\r\n",cmdPackPtr->cmdCode,cmdPackPtr->dataBodyLength);
    //搜索匹配处理器
    uint16_t indexTimer = 0;
    for(indexTimer = 0; indexTimer < DIM_ARRAY_ELEMENT_COUNT(uartCmdProcTimerUnitArray);indexTimer++)
    {
        if(cmdPackPtr->cmdCode == (uint32_t)(uartCmdProcTimerUnitArray[indexTimer].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexTimer == DIM_ARRAY_ELEMENT_COUNT(uartCmdProcTimerUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,cmdPackPtr);
        return;
    }
    //判定长度
    if(cmdPackPtr->dataBodyLength < uartCmdProcTimerUnitArray[indexTimer].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        uartCmdProcTimerUnitArray[indexTimer].exceptParamLengthMin,cmdPackPtr->dataBodyLength,
                        cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,cmdPackPtr);
        return;
    }
    //校验通过执行代码
    SYS_RESULT_PACK resultPack;
    uint8_t* resultDataBufferPtr = NULL;
    uint16_t resultDataLength = 0;
    //初始化错误返回包
    resultPack.resultCode = PROT_RESULT_FAIL;
    resultPack.errorMain = ERROR_MAIN_OK;
    resultPack.errorLevel = ERROR_LEVEL_NONE;
    resultPack.errorSub = ERROR_SUB_OK;
    //检测函数指针
    if(uartCmdProcTimerUnitArray[indexTimer].uartCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,uartCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,cmdPackPtr);
        return;
    }
    //调用处理函数
    uartCmdProcTimerUnitArray[indexTimer].uartCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(uartCmdProcTimerUnitArray[indexTimer].needResultReply == ENABLE)
    {
        //需要结果回传
        UartMasterSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
    }
    //释放内存
    if((resultDataBufferPtr != NULL)&&(resultDataLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,resultDataBufferPtr);
    }
    if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_UART,cmdPackPtr->dataBodyBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_UART,cmdPackPtr);
}

//占位函数
static void SrvMsgUartTimerCmd_PlaceHolder(UART_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

/****************************************网口上传消息处理函数**************************************************/
//占位函数
static void SrvMsgUartTimerUpload_PlaceHolder(UART_PROT_UPLOAD_PACK_RECV* uploadPackPtr);

//UART上传消息处理函数列表
static const USER_UART_UPLOAD_PROC_UNIT uartUploadTimerProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)UART_MASTER_REPORT_HOLD_PLACE     ,2 ,SrvMsgUartTimerUpload_PlaceHolder},
};

void SrvMsgUartTimerUpload(void* uartReportDataPtr)
{
    UART_PROT_UPLOAD_PACK_RECV* uartUploadPackRecvPtr = (UART_PROT_UPLOAD_PACK_RECV*)uartReportDataPtr;
    if(uartUploadPackRecvPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv uartUploadPackRecvPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexTimer = 0;
    for(indexTimer = 0; indexTimer < DIM_ARRAY_ELEMENT_COUNT(uartUploadTimerProcUnitArray);indexTimer++)
    {
        if(uartUploadPackRecvPtr->uploadID == (uint32_t)(uartUploadTimerProcUnitArray[indexTimer].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexTimer == DIM_ARRAY_ELEMENT_COUNT(uartUploadTimerProcUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,uartUploadPackRecvPtr->uploadID);
        //释放内存
        if((uartUploadPackRecvPtr->paramBufferPtr != NULL)&&(uartUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr);
        return;
    }
    //判定长度
    if(uartUploadPackRecvPtr->paramBufferLength < uartUploadTimerProcUnitArray[indexTimer].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        uartUploadTimerProcUnitArray[indexTimer].exceptParamLengthMin,uartUploadPackRecvPtr->paramBufferLength,
                        uartUploadPackRecvPtr->uploadID);
        //释放内存
        if((uartUploadPackRecvPtr->paramBufferPtr != NULL)&&(uartUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr);
        return;
    }
    //检测函数指针
    if(uartUploadTimerProcUnitArray[indexTimer].uartUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,uartUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        uartUploadPackRecvPtr->uploadID);
        //释放内存
        if((uartUploadPackRecvPtr->paramBufferPtr != NULL)&&(uartUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr);
        return;
    }
    //调用指令处理
    uartUploadTimerProcUnitArray[indexTimer].uartUploadProcFuncPtr(uartUploadPackRecvPtr);
    //处理完成,释放内存
    if((uartUploadPackRecvPtr->paramBufferPtr != NULL)&&(uartUploadPackRecvPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_UART,uartUploadPackRecvPtr);
}

//占位函数
static void SrvMsgUartTimerUpload_PlaceHolder(UART_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    asm("nop");
}







