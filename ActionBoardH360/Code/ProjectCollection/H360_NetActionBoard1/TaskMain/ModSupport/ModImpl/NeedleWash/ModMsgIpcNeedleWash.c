/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-27 13:03:14 +0800
************************************************************************************************/ 
#include "ModImplNeedleWash.h"
#include "ModTaskHeader.h"
#include "ModImplBaseNeedleWash.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void ModMsgIpcNeedleWashCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输入读取
static void ModMsgIpcNeedleWashCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输出写入
static void ModMsgIpcNeedleWashCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机复位
static void ModMsgIpcNeedleWashCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机走位
static void ModMsgIpcNeedleWashCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//复位
static void ModMsgIpcNeedleWashCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//清洗针灌注
static void ModMsgIpcNeedleWashCmd_Prime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//清洗针维护
static void ModMsgIpcNeedleWashCmd_Maintain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//清洗针磁珠清洗一次
static void ModMsgIpcNeedleWashCmd_BeadClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//清洗针下降到浸泡位
static void ModMsgIpcNeedleWashCmd_UpDownToSock(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//清洗针阻塞清洗
static void ModMsgIpcNeedleWashCmd_BlockMaintainClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//清洗针废液泵抽一次
static void ModMsgIpcNeedleWashCmd_PumpAbsorbOnce(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);

//CAN指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcNeedleWashMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF                                  ,0  ,ModMsgIpcNeedleWashCmd_CommTest           },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_PORT_IN                ,0  ,ModMsgIpcNeedleWashCmd_PortIn             },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_PORT_OUT               ,0  ,ModMsgIpcNeedleWashCmd_PortOut            },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_MOTOR_RESET            ,0  ,ModMsgIpcNeedleWashCmd_MotorReset         },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_MOTOR_RUN_STEPS        ,0  ,ModMsgIpcNeedleWashCmd_MotorRunSteps      },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_RESET                  ,0  ,ModMsgIpcNeedleWashCmd_Reset              },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_PRIME                  ,0  ,ModMsgIpcNeedleWashCmd_Prime              },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_MAINTAIN               ,0  ,ModMsgIpcNeedleWashCmd_Maintain           },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_BEAD_CLEAN             ,0  ,ModMsgIpcNeedleWashCmd_BeadClean          },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_UPDOWN_TO_SOCK         ,0  ,ModMsgIpcNeedleWashCmd_UpDownToSock       },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_BLOCK_MAINTAIN_CLEAN   ,0  ,ModMsgIpcNeedleWashCmd_BlockMaintainClean },
    {IPC_CMD_MACRO_H360_NEEDLE_WASH_PUMP_ABSORB_ONCE       ,0  ,ModMsgIpcNeedleWashCmd_PumpAbsorbOnce     },
};


//IPC指令消息处理函数
void ModMsgIpcNeedleWashCmd(void* ipcMsgBodyPtr)
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
    uint16_t indexNeedleWash = 0;
    for(indexNeedleWash = 0; indexNeedleWash < DIM_ARRAY_ELEMENT_COUNT(ipcNeedleWashMsgCmdProcUnitArray);indexNeedleWash++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcNeedleWashMsgCmdProcUnitArray[indexNeedleWash].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleWash == DIM_ARRAY_ELEMENT_COUNT(ipcNeedleWashMsgCmdProcUnitArray))
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
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcNeedleWashMsgCmdProcUnitArray[indexNeedleWash].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcNeedleWashMsgCmdProcUnitArray[indexNeedleWash].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
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
    if(ipcNeedleWashMsgCmdProcUnitArray[indexNeedleWash].ipcCmdProcFuncPtr == NULL)
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
    ipcNeedleWashMsgCmdProcUnitArray[indexNeedleWash].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
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
static void ModMsgIpcNeedleWashCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
static void ModMsgIpcNeedleWashCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内输出写入
static void ModMsgIpcNeedleWashCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机复位
static void ModMsgIpcNeedleWashCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机走位
static void ModMsgIpcNeedleWashCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//复位
static void ModMsgIpcNeedleWashCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//清洗针灌注
static void ModMsgIpcNeedleWashCmd_Prime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//清洗针维护
static void ModMsgIpcNeedleWashCmd_Maintain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//清洗针磁珠清洗一次
static void ModMsgIpcNeedleWashCmd_BeadClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//清洗针下降到浸泡位
static void ModMsgIpcNeedleWashCmd_UpDownToSock(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//清洗针阻塞清洗
static void ModMsgIpcNeedleWashCmd_BlockMaintainClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//清洗针废液泵抽一次
static void ModMsgIpcNeedleWashCmd_PumpAbsorbOnce(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void ModMsgIpcNeedleWashUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcNeedleWashMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,ModMsgIpcNeedleWashUpload_PlaceHolder},
};


//IPC上传消息处理函数
void ModMsgIpcNeedleWashUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexNeedleWash = 0;
    for(indexNeedleWash = 0; indexNeedleWash < DIM_ARRAY_ELEMENT_COUNT(ipcNeedleWashMsgUploadProcUnitArray);indexNeedleWash++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcNeedleWashMsgUploadProcUnitArray[indexNeedleWash].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleWash == DIM_ARRAY_ELEMENT_COUNT(ipcNeedleWashMsgUploadProcUnitArray))
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
    if(ipcUploadDataPtr->dataLength < ipcNeedleWashMsgUploadProcUnitArray[indexNeedleWash].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcNeedleWashMsgUploadProcUnitArray[indexNeedleWash].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
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
    if(ipcNeedleWashMsgUploadProcUnitArray[indexNeedleWash].ipcUploadProcFuncPtr == NULL)
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
    ipcNeedleWashMsgUploadProcUnitArray[indexNeedleWash].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void ModMsgIpcNeedleWashUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/




