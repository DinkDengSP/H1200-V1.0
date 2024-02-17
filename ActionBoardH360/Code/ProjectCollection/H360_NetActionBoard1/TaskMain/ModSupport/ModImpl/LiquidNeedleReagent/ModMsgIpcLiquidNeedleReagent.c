/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 23:45:56 +0800
************************************************************************************************/ 
#include "ModImplLiquidNeedleReagent.h"
#include "ModTaskHeader.h"
#include "ModImplBaseLiquidNeedleReagent.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void ModMsgIpcLiquidNeedleReagentCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输入读取
static void ModMsgIpcLiquidNeedleReagentCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输出写入
static void ModMsgIpcLiquidNeedleReagentCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机复位
static void ModMsgIpcLiquidNeedleReagentCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机走位
static void ModMsgIpcLiquidNeedleReagentCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//复位
static void ModMsgIpcLiquidNeedleReagentCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//打开清洗液路
static void ModMsgIpcLiquidNeedleReagentCmd_OpenClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//关闭清洗液路
static void ModMsgIpcLiquidNeedleReagentCmd_CloseClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//打开灌注液路
static void ModMsgIpcLiquidNeedleReagentCmd_OpenPrime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//关闭灌注液路
static void ModMsgIpcLiquidNeedleReagentCmd_ClosePrime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//打开清洗维护液路
static void ModMsgIpcLiquidNeedleReagentCmd_OpenMaintain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//关闭清洗维护液路
static void ModMsgIpcLiquidNeedleReagentCmd_CloseMaintain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);


//CAN指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcLiquidNeedleReagentMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF                                    ,0  ,ModMsgIpcLiquidNeedleReagentCmd_CommTest     },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_PORT_IN           ,0  ,ModMsgIpcLiquidNeedleReagentCmd_PortIn       },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_PORT_OUT          ,0  ,ModMsgIpcLiquidNeedleReagentCmd_PortOut      },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_MOTOR_RESET       ,0  ,ModMsgIpcLiquidNeedleReagentCmd_MotorReset   },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_MOTOR_RUN_STEPS   ,0  ,ModMsgIpcLiquidNeedleReagentCmd_MotorRunSteps},
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_RESET             ,0  ,ModMsgIpcLiquidNeedleReagentCmd_Reset        },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_OPEN_CLEAN        ,0  ,ModMsgIpcLiquidNeedleReagentCmd_OpenClean    },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_CLOSE_CLEAN       ,0  ,ModMsgIpcLiquidNeedleReagentCmd_CloseClean   },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_OPEN_PRIME        ,0  ,ModMsgIpcLiquidNeedleReagentCmd_OpenPrime    },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_CLOSE_PRIME       ,0  ,ModMsgIpcLiquidNeedleReagentCmd_ClosePrime   },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_OPEN_MAINTAIN     ,0  ,ModMsgIpcLiquidNeedleReagentCmd_OpenMaintain },
    {IPC_CMD_MACRO_H360_LIQUID_NDL_REAGENT_CLOSE_MAINTAIN    ,0  ,ModMsgIpcLiquidNeedleReagentCmd_CloseMaintain},
};


//IPC指令消息处理函数
void ModMsgIpcLiquidNeedleReagentCmd(void* ipcMsgBodyPtr)
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
    uint16_t indexLiquidNeedleReagent = 0;
    for(indexLiquidNeedleReagent = 0; indexLiquidNeedleReagent < DIM_ARRAY_ELEMENT_COUNT(ipcLiquidNeedleReagentMsgCmdProcUnitArray);indexLiquidNeedleReagent++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcLiquidNeedleReagentMsgCmdProcUnitArray[indexLiquidNeedleReagent].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexLiquidNeedleReagent == DIM_ARRAY_ELEMENT_COUNT(ipcLiquidNeedleReagentMsgCmdProcUnitArray))
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
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcLiquidNeedleReagentMsgCmdProcUnitArray[indexLiquidNeedleReagent].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcLiquidNeedleReagentMsgCmdProcUnitArray[indexLiquidNeedleReagent].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
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
    if(ipcLiquidNeedleReagentMsgCmdProcUnitArray[indexLiquidNeedleReagent].ipcCmdProcFuncPtr == NULL)
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
    ipcLiquidNeedleReagentMsgCmdProcUnitArray[indexLiquidNeedleReagent].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
    //处理完成回传
    ipcWaitResultPackPtr->resultPack.resultCode = sysResultPack.resultCode;
    ipcWaitResultPackPtr->resultPack.errorMain = sysResultPack.errorMain;
    ipcWaitResultPackPtr->resultPack.errorLevel = sysResultPack.errorLevel;
    ipcWaitResultPackPtr->resultPack.errorSub = sysResultPack.errorSub;
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
static void ModMsgIpcLiquidNeedleReagentCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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

//模块内输入读取
static void ModMsgIpcLiquidNeedleReagentCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内输出写入
static void ModMsgIpcLiquidNeedleReagentCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机复位
static void ModMsgIpcLiquidNeedleReagentCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机走位
static void ModMsgIpcLiquidNeedleReagentCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//复位
static void ModMsgIpcLiquidNeedleReagentCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//打开清洗液路
static void ModMsgIpcLiquidNeedleReagentCmd_OpenClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//关闭清洗液路
static void ModMsgIpcLiquidNeedleReagentCmd_CloseClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//打开灌注液路
static void ModMsgIpcLiquidNeedleReagentCmd_OpenPrime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//关闭灌注液路
static void ModMsgIpcLiquidNeedleReagentCmd_ClosePrime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//打开清洗维护液路
static void ModMsgIpcLiquidNeedleReagentCmd_OpenMaintain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//关闭清洗维护液路
static void ModMsgIpcLiquidNeedleReagentCmd_CloseMaintain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void ModMsgIpcLiquidNeedleReagentUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcLiquidNeedleReagentMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,ModMsgIpcLiquidNeedleReagentUpload_PlaceHolder},
};


//IPC上传消息处理函数
void ModMsgIpcLiquidNeedleReagentUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexLiquidNeedleReagent = 0;
    for(indexLiquidNeedleReagent = 0; indexLiquidNeedleReagent < DIM_ARRAY_ELEMENT_COUNT(ipcLiquidNeedleReagentMsgUploadProcUnitArray);indexLiquidNeedleReagent++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcLiquidNeedleReagentMsgUploadProcUnitArray[indexLiquidNeedleReagent].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexLiquidNeedleReagent == DIM_ARRAY_ELEMENT_COUNT(ipcLiquidNeedleReagentMsgUploadProcUnitArray))
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
    if(ipcUploadDataPtr->dataLength < ipcLiquidNeedleReagentMsgUploadProcUnitArray[indexLiquidNeedleReagent].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcLiquidNeedleReagentMsgUploadProcUnitArray[indexLiquidNeedleReagent].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
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
    if(ipcLiquidNeedleReagentMsgUploadProcUnitArray[indexLiquidNeedleReagent].ipcUploadProcFuncPtr == NULL)
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
    ipcLiquidNeedleReagentMsgUploadProcUnitArray[indexLiquidNeedleReagent].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void ModMsgIpcLiquidNeedleReagentUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/




