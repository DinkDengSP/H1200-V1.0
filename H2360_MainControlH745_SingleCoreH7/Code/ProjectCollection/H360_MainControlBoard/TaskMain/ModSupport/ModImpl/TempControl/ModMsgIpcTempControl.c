/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-09 16:40:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-08 10:26:38 +0800
************************************************************************************************/ 
#include "ModImplTempControl.h"
#include "ModImplBaseTempControl.h"
#include "ModTaskHeader.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void ModMsgIpcTempControlCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//温控模块读取反应盘温度
static void ModMsgIpcTempControlCmd_GetPlateReactionTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//温控读取试剂盘温度
static void ModMsgIpcTempControlCmd_GetPlateReagentTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//温控读取清洗盘温度
static void ModMsgIpcTempControlCmd_GetPlateWashTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//温控读取环境温度
static void ModMsgIpcTempControlCmd_GetEnvirTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//温控读取板上温度
static void ModMsgIpcTempControlCmd_GetBoardTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);


//IPC指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcTempControlMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF                                                            ,0    ,ModMsgIpcTempControlCmd_CommTest            },
    {(uint32_t)IPC_MOD_CMD_MACRO_TEMPERATURE_CONTROL_READ_CHANNEL_PLATE_REACTION     ,0    ,ModMsgIpcTempControlCmd_GetPlateReactionTemp},
    {(uint32_t)IPC_MOD_CMD_MACRO_TEMPERATURE_CONTROL_READ_CHANNEL_PLATE_REAGENT      ,0    ,ModMsgIpcTempControlCmd_GetPlateReagentTemp },
    {(uint32_t)IPC_MOD_CMD_MACRO_TEMPERATURE_CONTROL_READ_CHANNEL_PLATE_WASH         ,0    ,ModMsgIpcTempControlCmd_GetPlateWashTemp    },
    {(uint32_t)IPC_MOD_CMD_MACRO_TEMPERATURE_CONTROL_READ_CHANNEL_ENVIR              ,0    ,ModMsgIpcTempControlCmd_GetEnvirTemp        },
    {(uint32_t)IPC_MOD_CMD_MACRO_TEMPERATURE_CONTROL_READ_CHANNEL_BOARD              ,0    ,ModMsgIpcTempControlCmd_GetBoardTemp        },
};


//IPC指令消息处理函数
void ModMsgIpcTempControlCmd(void* ipcMsgBodyPtr)
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
    uint16_t indexTempControl = 0;
    for(indexTempControl = 0; indexTempControl < DIM_ARRAY_ELEMENT_COUNT(ipcTempControlMsgCmdProcUnitArray);indexTempControl++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcTempControlMsgCmdProcUnitArray[indexTempControl].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexTempControl == DIM_ARRAY_ELEMENT_COUNT(ipcTempControlMsgCmdProcUnitArray))
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
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcTempControlMsgCmdProcUnitArray[indexTempControl].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcTempControlMsgCmdProcUnitArray[indexTempControl].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
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
    if(ipcTempControlMsgCmdProcUnitArray[indexTempControl].ipcCmdProcFuncPtr == NULL)
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
    ipcTempControlMsgCmdProcUnitArray[indexTempControl].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
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
static void ModMsgIpcTempControlCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_MOD_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    //回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//温控模块读取反应盘温度
static void ModMsgIpcTempControlCmd_GetPlateReactionTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_MOD_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    ModImplBaseTempControlGetPlateReactionTemp(resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //结果
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,
                                returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CARRY_BUFFER_NOT_ENOUGH,
                                                ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
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
        UserMemFree(MEM_REGION_MOD_TASK,returnBufferPtr);
    }
}

//温控读取试剂盘温度
static void ModMsgIpcTempControlCmd_GetPlateReagentTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_MOD_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    ModImplBaseTempControlGetPlateReagentTemp(resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //结果
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,
                                returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CARRY_BUFFER_NOT_ENOUGH,
                                                ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
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
        UserMemFree(MEM_REGION_MOD_TASK,returnBufferPtr);
    }
}

//温控读取清洗盘温度
static void ModMsgIpcTempControlCmd_GetPlateWashTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_MOD_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    ModImplBaseTempControlGetPlateWashTemp(resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //结果
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,
                                returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CARRY_BUFFER_NOT_ENOUGH,
                                                ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
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
        UserMemFree(MEM_REGION_MOD_TASK,returnBufferPtr);
    }
}

//温控读取环境温度
static void ModMsgIpcTempControlCmd_GetEnvirTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_MOD_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    ModImplBaseTempControlGetEnvirTemp(resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //结果
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,
                                returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CARRY_BUFFER_NOT_ENOUGH,
                                                ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
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
        UserMemFree(MEM_REGION_MOD_TASK,returnBufferPtr);
    }
}

//温控读取板上温度
static void ModMsgIpcTempControlCmd_GetBoardTemp(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_MOD_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    ModImplBaseTempControlGetBoardTemp(resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //结果
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,
                                returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CARRY_BUFFER_NOT_ENOUGH,
                                                ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
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
        UserMemFree(MEM_REGION_MOD_TASK,returnBufferPtr);
    }
}



/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void ModMsgIpcTempControlUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcTempControlMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,ModMsgIpcTempControlUpload_PlaceHolder},
};


//IPC上传消息处理函数
void ModMsgIpcTempControlUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexTempControl = 0;
    for(indexTempControl = 0; indexTempControl < DIM_ARRAY_ELEMENT_COUNT(ipcTempControlMsgUploadProcUnitArray);indexTempControl++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcTempControlMsgUploadProcUnitArray[indexTempControl].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexTempControl == DIM_ARRAY_ELEMENT_COUNT(ipcTempControlMsgUploadProcUnitArray))
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
    if(ipcUploadDataPtr->dataLength < ipcTempControlMsgUploadProcUnitArray[indexTempControl].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcTempControlMsgUploadProcUnitArray[indexTempControl].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
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
    if(ipcTempControlMsgUploadProcUnitArray[indexTempControl].ipcUploadProcFuncPtr == NULL)
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
    ipcTempControlMsgUploadProcUnitArray[indexTempControl].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void ModMsgIpcTempControlUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/






















