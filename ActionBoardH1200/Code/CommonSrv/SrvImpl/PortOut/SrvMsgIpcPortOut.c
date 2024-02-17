/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 11:37:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplPortOut.h"
#include "SrvImplBasePortOut.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void SrvMsgIpcPortOutCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//单个输入写入
static void SrvMsgIpcPortOutCmd_WriteSingle(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//批量输入写入
static void SrvMsgIpcPortOutCmd_WriteAny(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);

//IPC指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcPortOutMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF     ,0    ,SrvMsgIpcPortOutCmd_CommTest       },
    {(uint32_t)0X00000000     ,0    ,SrvMsgIpcPortOutCmd_WriteSingle    },
    {(uint32_t)0X00000001     ,0    ,SrvMsgIpcPortOutCmd_WriteAny       },
};


//IPC指令消息处理函数
void SrvMsgIpcPortOutCmd(void* ipcMsgBodyPtr)
{
    //转换指令码
    IPC_CMD_DATA* ipcCmdMsgPtr = (IPC_CMD_DATA*)ipcMsgBodyPtr;
    if(ipcCmdMsgPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcCmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    IPC_CMD_WAIT_RESULT* ipcWaitResultPackPtr = ipcCmdMsgPtr->waitResultPackPtr;
    //搜索匹配处理器
    uint16_t indexPortOut = 0;
    for(indexPortOut = 0; indexPortOut < DIM_ARRAY_ELEMENT_COUNT(ipcPortOutMsgCmdProcUnitArray);indexPortOut++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcPortOutMsgCmdProcUnitArray[indexPortOut].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexPortOut == DIM_ARRAY_ELEMENT_COUNT(ipcPortOutMsgCmdProcUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_ERROR_SUPPORT;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //判定长度
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcPortOutMsgCmdProcUnitArray[indexPortOut].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcPortOutMsgCmdProcUnitArray[indexPortOut].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
                        ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_CMD_PAR_LEN;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //校验通过执行代码
    SYS_RESULT_PACK sysResultPack;
    //初始化错误返回包
    sysResultPack.resultCode = PROT_RESULT_FAIL;
    sysResultPack.errorMain = ERROR_MAIN_OK;
    sysResultPack.errorLevel = ERROR_LEVEL_NONE;
    sysResultPack.errorSub = ERROR_SUB_OK;
    //检测函数指针
    if(ipcPortOutMsgCmdProcUnitArray[indexPortOut].ipcCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,ipcCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_PROC_FUNC_NULL;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //调用处理函数
    ipcPortOutMsgCmdProcUnitArray[indexPortOut].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
    //处理完成回传
    ipcWaitResultPackPtr->resultPack.resultCode = sysResultPack.resultCode;
    ipcWaitResultPackPtr->resultPack.errorMain = sysResultPack.errorMain;
    ipcWaitResultPackPtr->resultPack.errorLevel = sysResultPack.errorLevel;
    ipcWaitResultPackPtr->resultPack.errorSub = sysResultPack.errorSub;
    //打印错误
    if(ipcWaitResultPackPtr->resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("%s,Cmd: 0X%04X,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,ipcCmdMsgPtr->cmdBaseData.commandCode,sysResultPack.errorMain,sysResultPack.errorLevel,
                        sysResultPack.errorSub);
    }
    //释放内存
    if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
    {
        IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
    }
    IPC_PortMemFree(ipcCmdMsgPtr);
    //状态设置完成
    ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_OVER;
    return;
}

//通讯测试
static void SrvMsgIpcPortOutCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    //回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//单个输入写入
static void SrvMsgIpcPortOutCmd_WriteSingle(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    uint8_t writeSingleIndex = (uint8_t)(ipcCmdMsgPtr->cmdParam1);
    uint8_t writeSingleVal = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s,writeSingleIndex : %d,writeSingleVal: %d\r\n",__func__,writeSingleIndex,writeSingleVal);
#endif
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBasePortOutWriteSingle(writeSingleIndex,writeSingleVal,resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PORT_WRITE,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//批量输入写入
static void SrvMsgIpcPortOutCmd_WriteAny(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    uint8_t writeSinglePair = (uint8_t)(ipcCmdMsgPtr->cmdParamBufferLength/2);
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s,writeSinglePair : %d\r\n",__func__,writeSinglePair);
#endif
    uint8_t* writePairDataArrayPtr = ipcCmdMsgPtr->cmdParamBufferPtr;
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBasePortOutWriteAny(writeSinglePair,writePairDataArrayPtr,resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_PORT_WRITE,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void SrvMsgIpcPortOutUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcPortOutMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,SrvMsgIpcPortOutUpload_PlaceHolder},
};


//IPC上传消息处理函数
void SrvMsgIpcPortOutUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexPortOut = 0;
    for(indexPortOut = 0; indexPortOut < DIM_ARRAY_ELEMENT_COUNT(ipcPortOutMsgUploadProcUnitArray);indexPortOut++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcPortOutMsgUploadProcUnitArray[indexPortOut].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexPortOut == DIM_ARRAY_ELEMENT_COUNT(ipcPortOutMsgUploadProcUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //判定长度
    if(ipcUploadDataPtr->dataLength < ipcPortOutMsgUploadProcUnitArray[indexPortOut].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcPortOutMsgUploadProcUnitArray[indexPortOut].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
                        ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //检测函数指针
    if(ipcPortOutMsgUploadProcUnitArray[indexPortOut].ipcUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,ipcUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //调用指令处理
    ipcPortOutMsgUploadProcUnitArray[indexPortOut].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void SrvMsgIpcPortOutUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/







