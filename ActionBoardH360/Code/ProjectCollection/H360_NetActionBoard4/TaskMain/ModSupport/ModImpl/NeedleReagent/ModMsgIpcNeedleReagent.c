/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-09 16:40:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-28 11:01:24 +0800
************************************************************************************************/ 
#include "ModImplNeedleReagent.h"
#include "ModTaskHeader.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void ModMsgIpcNeedleReagentCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输入读取
static void ModMsgIpcNeedleReagentCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输出写入
static void ModMsgIpcNeedleReagentCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机复位
static void ModMsgIpcNeedleReagentCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机走位
static void ModMsgIpcNeedleReagentCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针复位,带零位偏移
static void ModMsgIpcNeedleReagentCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针清洗
static void ModMsgIpcNeedleReagentCmd_Clean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针灌注
static void ModMsgIpcNeedleReagentCmd_Prime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针清洗维护
static void ModMsgIpcNeedleReagentCmd_MaintainClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针旋转指定位置
static void ModMsgIpcNeedleReagentCmd_Rotate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针吸液
static void ModMsgIpcNeedleReagentCmd_LiquidAbsorb(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针注液
static void ModMsgIpcNeedleReagentCmd_LiquidInject(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针探液校准
static void ModMsgIpcNeedleReagentCmd_LiquidDetectAdjust(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读试剂针探液板状态
static void ModMsgIpcNeedleReagentCmd_LiquidDetectReadState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针读取针尖电阻和电压
static void ModMsgIpcNeedleReagentCmd_LiquidDetectReadResAndVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针写入探液板电阻
static void ModMsgIpcNeedleReagentCmd_LiquidDetectWriteRes(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针读取针尖基准电压
static void ModMsgIpcNeedleReagentCmd_LiquidDetectReadBaseVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针液量余量检查
static void ModMsgIpcNeedleReagentCmd_LiquidRedidueCheck(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//试剂针升降急停高度读取
static void ModMsgIpcNeedleReagentCmd_ReadNeedleUpDownEmStopPos(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读取探液板版本号
static void ModMsgIpcNeedleReagentCmd_ReadLiquidBoardVersion(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);

//CAN指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcNeedleReagentMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF                                              ,0  ,ModMsgIpcNeedleReagentCmd_CommTest                    },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_PORT_IN                         ,0  ,ModMsgIpcNeedleReagentCmd_PortIn                      },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_PORT_OUT                        ,0  ,ModMsgIpcNeedleReagentCmd_PortOut                     },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_MOTOR_RESET                     ,0  ,ModMsgIpcNeedleReagentCmd_MotorReset                  },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_MOTOR_RUN_STEPS                 ,0  ,ModMsgIpcNeedleReagentCmd_MotorRunSteps               },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_RESET                           ,0  ,ModMsgIpcNeedleReagentCmd_Reset                       },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_CLEAN                           ,0  ,ModMsgIpcNeedleReagentCmd_Clean                       },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_PRIME                           ,0  ,ModMsgIpcNeedleReagentCmd_Prime                       },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_MAINTAIN_CLEAN                  ,0  ,ModMsgIpcNeedleReagentCmd_MaintainClean               },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_ROTATE                          ,0  ,ModMsgIpcNeedleReagentCmd_Rotate                      },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_LIQUID_ABSORB                   ,0  ,ModMsgIpcNeedleReagentCmd_LiquidAbsorb                },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_LIQUID_INJECT                   ,0  ,ModMsgIpcNeedleReagentCmd_LiquidInject                },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST            ,0  ,ModMsgIpcNeedleReagentCmd_LiquidDetectAdjust          },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE        ,0  ,ModMsgIpcNeedleReagentCmd_LiquidDetectReadState       },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_RES_AND_VOL  ,0  ,ModMsgIpcNeedleReagentCmd_LiquidDetectReadResAndVol   },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_LIQUID_DETECT_WRITE_RES         ,0  ,ModMsgIpcNeedleReagentCmd_LiquidDetectWriteRes        },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_BASE_VOL     ,0  ,ModMsgIpcNeedleReagentCmd_LiquidDetectReadBaseVol     },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK            ,0  ,ModMsgIpcNeedleReagentCmd_LiquidRedidueCheck          },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_READ_NEEDLE_UP_DOWN_EM_STOP_POS ,0  ,ModMsgIpcNeedleReagentCmd_ReadNeedleUpDownEmStopPos   },
    {IPC_CMD_MACRO_H360_NEEDLE_REAGENT_READ_LIQUID_BOARD_VERSION       ,0  ,ModMsgIpcNeedleReagentCmd_ReadLiquidBoardVersion      },
};


//IPC指令消息处理函数
void ModMsgIpcNeedleReagentCmd(void* ipcMsgBodyPtr)
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
    uint16_t indexNeedleReagent = 0;
    for(indexNeedleReagent = 0; indexNeedleReagent < DIM_ARRAY_ELEMENT_COUNT(ipcNeedleReagentMsgCmdProcUnitArray);indexNeedleReagent++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcNeedleReagentMsgCmdProcUnitArray[indexNeedleReagent].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleReagent == DIM_ARRAY_ELEMENT_COUNT(ipcNeedleReagentMsgCmdProcUnitArray))
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
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcNeedleReagentMsgCmdProcUnitArray[indexNeedleReagent].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcNeedleReagentMsgCmdProcUnitArray[indexNeedleReagent].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
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
    if(ipcNeedleReagentMsgCmdProcUnitArray[indexNeedleReagent].ipcCmdProcFuncPtr == NULL)
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
    ipcNeedleReagentMsgCmdProcUnitArray[indexNeedleReagent].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
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
static void ModMsgIpcNeedleReagentCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
static void ModMsgIpcNeedleReagentCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内输出写入
static void ModMsgIpcNeedleReagentCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机复位
static void ModMsgIpcNeedleReagentCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机走位
static void ModMsgIpcNeedleReagentCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针复位,带零位偏移
static void ModMsgIpcNeedleReagentCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针清洗
static void ModMsgIpcNeedleReagentCmd_Clean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针灌注
static void ModMsgIpcNeedleReagentCmd_Prime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针清洗维护
static void ModMsgIpcNeedleReagentCmd_MaintainClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针旋转指定位置
static void ModMsgIpcNeedleReagentCmd_Rotate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针吸液
static void ModMsgIpcNeedleReagentCmd_LiquidAbsorb(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针注液
static void ModMsgIpcNeedleReagentCmd_LiquidInject(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针探液校准
static void ModMsgIpcNeedleReagentCmd_LiquidDetectAdjust(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//读试剂针探液板状态
static void ModMsgIpcNeedleReagentCmd_LiquidDetectReadState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针读取针尖电阻和电压
static void ModMsgIpcNeedleReagentCmd_LiquidDetectReadResAndVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针写入探液板电阻
static void ModMsgIpcNeedleReagentCmd_LiquidDetectWriteRes(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针读取针尖基准电压
static void ModMsgIpcNeedleReagentCmd_LiquidDetectReadBaseVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针液量余量检查
static void ModMsgIpcNeedleReagentCmd_LiquidRedidueCheck(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//试剂针升降急停高度读取
static void ModMsgIpcNeedleReagentCmd_ReadNeedleUpDownEmStopPos(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//读取探液板版本号
static void ModMsgIpcNeedleReagentCmd_ReadLiquidBoardVersion(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}


/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void ModMsgIpcNeedleReagentUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcNeedleReagentMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,ModMsgIpcNeedleReagentUpload_PlaceHolder},
};


//IPC上传消息处理函数
void ModMsgIpcNeedleReagentUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexNeedleReagent = 0;
    for(indexNeedleReagent = 0; indexNeedleReagent < DIM_ARRAY_ELEMENT_COUNT(ipcNeedleReagentMsgUploadProcUnitArray);indexNeedleReagent++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcNeedleReagentMsgUploadProcUnitArray[indexNeedleReagent].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleReagent == DIM_ARRAY_ELEMENT_COUNT(ipcNeedleReagentMsgUploadProcUnitArray))
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
    if(ipcUploadDataPtr->dataLength < ipcNeedleReagentMsgUploadProcUnitArray[indexNeedleReagent].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcNeedleReagentMsgUploadProcUnitArray[indexNeedleReagent].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
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
    if(ipcNeedleReagentMsgUploadProcUnitArray[indexNeedleReagent].ipcUploadProcFuncPtr == NULL)
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
    ipcNeedleReagentMsgUploadProcUnitArray[indexNeedleReagent].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void ModMsgIpcNeedleReagentUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/






















