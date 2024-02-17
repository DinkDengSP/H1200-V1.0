/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-28 13:58:43 +0800
************************************************************************************************/ 
#include "ModImplHandNewCup.h"
#include "ModTaskHeader.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void ModMsgIpcHandNewCupCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输入读取
static void ModMsgIpcHandNewCupCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输出写入
static void ModMsgIpcHandNewCupCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机复位
static void ModMsgIpcHandNewCupCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机走位
static void ModMsgIpcHandNewCupCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯机械手复位
static void ModMsgIpcHandNewCupCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯机械手移动到新杯区
static void ModMsgIpcHandNewCupCmd_MoveToNewCupTray(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯机械手移动到反应盘
static void ModMsgIpcHandNewCupCmd_MoveToPlateReaction(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯机械手移动到垃圾桶
static void ModMsgIpcHandNewCupCmd_MoveToGarbage(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯机械手新杯区取杯
static void ModMsgIpcHandNewCupCmd_CatchCupNewTray(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯机械手反应盘取杯
static void ModMsgIpcHandNewCupCmd_CatchCupPlate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯机械手反应盘放杯
static void ModMsgIpcHandNewCupCmd_PutCupPlate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯机械手垃圾桶放杯
static void ModMsgIpcHandNewCupCmd_PutCupGarbage(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);

//CAN指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcHandNewCupMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF                                      ,0  ,ModMsgIpcHandNewCupCmd_CommTest           },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_PORT_IN                   ,0  ,ModMsgIpcHandNewCupCmd_PortIn             },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_PORT_OUT                  ,0  ,ModMsgIpcHandNewCupCmd_PortOut            },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_MOTOR_RESET               ,0  ,ModMsgIpcHandNewCupCmd_MotorReset         },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_MOTOR_RUN_STEPS           ,0  ,ModMsgIpcHandNewCupCmd_MotorRunSteps      },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_RESET                     ,0  ,ModMsgIpcHandNewCupCmd_Reset              },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_MOVE_TO_NEW_CUP_TRAY      ,0  ,ModMsgIpcHandNewCupCmd_MoveToNewCupTray   },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_MOVE_TO_PLATE_REACTION    ,0  ,ModMsgIpcHandNewCupCmd_MoveToPlateReaction},
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_MOVE_TO_GARBAGE           ,0  ,ModMsgIpcHandNewCupCmd_MoveToGarbage      },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_CATCH_CUP_NEW_TRAY        ,0  ,ModMsgIpcHandNewCupCmd_CatchCupNewTray    },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_CATCH_CUP_PLATE           ,0  ,ModMsgIpcHandNewCupCmd_CatchCupPlate      },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_PUT_CUP_PLATE             ,0  ,ModMsgIpcHandNewCupCmd_PutCupPlate        },
    {IPC_CMD_MACRO_H360_HAND_NEW_CUP_PUT_CUP_GARBAGE           ,0  ,ModMsgIpcHandNewCupCmd_PutCupGarbage      },
};


//IPC指令消息处理函数
void ModMsgIpcHandNewCupCmd(void* ipcMsgBodyPtr)
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
    uint16_t indexHandNewCup = 0;
    for(indexHandNewCup = 0; indexHandNewCup < DIM_ARRAY_ELEMENT_COUNT(ipcHandNewCupMsgCmdProcUnitArray);indexHandNewCup++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcHandNewCupMsgCmdProcUnitArray[indexHandNewCup].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexHandNewCup == DIM_ARRAY_ELEMENT_COUNT(ipcHandNewCupMsgCmdProcUnitArray))
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
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcHandNewCupMsgCmdProcUnitArray[indexHandNewCup].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcHandNewCupMsgCmdProcUnitArray[indexHandNewCup].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
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
    if(ipcHandNewCupMsgCmdProcUnitArray[indexHandNewCup].ipcCmdProcFuncPtr == NULL)
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
    ipcHandNewCupMsgCmdProcUnitArray[indexHandNewCup].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
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
static void ModMsgIpcHandNewCupCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
static void ModMsgIpcHandNewCupCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内输出写入
static void ModMsgIpcHandNewCupCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机复位
static void ModMsgIpcHandNewCupCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机走位
static void ModMsgIpcHandNewCupCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯机械手复位
static void ModMsgIpcHandNewCupCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯机械手移动到新杯区
static void ModMsgIpcHandNewCupCmd_MoveToNewCupTray(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯机械手移动到反应盘
static void ModMsgIpcHandNewCupCmd_MoveToPlateReaction(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯机械手移动到垃圾桶
static void ModMsgIpcHandNewCupCmd_MoveToGarbage(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯机械手新杯区取杯
static void ModMsgIpcHandNewCupCmd_CatchCupNewTray(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯机械手反应盘取杯
static void ModMsgIpcHandNewCupCmd_CatchCupPlate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯机械手反应盘放杯
static void ModMsgIpcHandNewCupCmd_PutCupPlate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯机械手垃圾桶放杯
static void ModMsgIpcHandNewCupCmd_PutCupGarbage(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void ModMsgIpcHandNewCupUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcHandNewCupMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,ModMsgIpcHandNewCupUpload_PlaceHolder},
};


//IPC上传消息处理函数
void ModMsgIpcHandNewCupUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexHandNewCup = 0;
    for(indexHandNewCup = 0; indexHandNewCup < DIM_ARRAY_ELEMENT_COUNT(ipcHandNewCupMsgUploadProcUnitArray);indexHandNewCup++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcHandNewCupMsgUploadProcUnitArray[indexHandNewCup].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexHandNewCup == DIM_ARRAY_ELEMENT_COUNT(ipcHandNewCupMsgUploadProcUnitArray))
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
    if(ipcUploadDataPtr->dataLength < ipcHandNewCupMsgUploadProcUnitArray[indexHandNewCup].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcHandNewCupMsgUploadProcUnitArray[indexHandNewCup].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
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
    if(ipcHandNewCupMsgUploadProcUnitArray[indexHandNewCup].ipcUploadProcFuncPtr == NULL)
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
    ipcHandNewCupMsgUploadProcUnitArray[indexHandNewCup].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void ModMsgIpcHandNewCupUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/















