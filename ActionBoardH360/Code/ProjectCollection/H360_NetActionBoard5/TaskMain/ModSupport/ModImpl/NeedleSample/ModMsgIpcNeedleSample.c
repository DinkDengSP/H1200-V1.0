/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-28 14:01:11 +0800
************************************************************************************************/ 
#include "ModImplNeedleSample.h"
#include "ModTaskHeader.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void ModMsgIpcNeedleSampleCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输入读取
static void ModMsgIpcNeedleSampleCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输出写入
static void ModMsgIpcNeedleSampleCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机复位
static void ModMsgIpcNeedleSampleCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机走位
static void ModMsgIpcNeedleSampleCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针复位
static void ModMsgIpcNeedleSampleCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针清洗液灌注
static void ModMsgIpcNeedleSampleCmd_PrimeNormal(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针强力清洗液灌注
static void ModMsgIpcNeedleSampleCmd_PrimePower(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针清洗
static void ModMsgIpcNeedleSampleCmd_Clean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针强力清洗
static void ModMsgIpcNeedleSampleCmd_CleanPower(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针清洗维护
static void ModMsgIpcNeedleSampleCmd_MainTain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针旋转指定位置
static void ModMsgIpcNeedleSampleCmd_Rotate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针吸试管样本
static void ModMsgIpcNeedleSampleCmd_LiquidAbsorbTube(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针吸反应盘样本
static void ModMsgIpcNeedleSampleCmd_LiquidAbsorbPlate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针注液
static void ModMsgIpcNeedleSampleCmd_InjectNormal(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针携带维护
static void ModMsgIpcNeedleSampleCmd_InjectMaintain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//样本针探液校准
static void ModMsgIpcNeedleSampleCmd_LiquidDetectAdjust(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读样本针探液板状态
static void ModMsgIpcNeedleSampleCmd_LiquidDetectReadState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读取电阻和针尖电压
static void ModMsgIpcNeedleSampleCmd_LiquidDetectReadResAndVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//写入电阻值
static void ModMsgIpcNeedleSampleCmd_LiquidDetectWriteRes(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读取针尖基准电压
static void ModMsgIpcNeedleSampleCmd_LiquidDetectReadBaseVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读样本针升降急停高度
static void ModMsgIpcNeedleSampleCmd_ReadNeedleUpDownEmStopPos(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读探液板程序版本
static void ModMsgIpcNeedleSampleCmd_ReadLiquidBoardVersion(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);

//CAN指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcNeedleSampleMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF                                                ,0  ,ModMsgIpcNeedleSampleCmd_CommTest                   },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_PORT_IN                            ,0  ,ModMsgIpcNeedleSampleCmd_PortIn                     },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_PORT_OUT                           ,0  ,ModMsgIpcNeedleSampleCmd_PortOut                    },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_MOTOR_RESET                        ,0  ,ModMsgIpcNeedleSampleCmd_MotorReset                 },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_MOTOR_RUN_STEPS                    ,0  ,ModMsgIpcNeedleSampleCmd_MotorRunSteps              },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_RESET                              ,0  ,ModMsgIpcNeedleSampleCmd_Reset                      },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_PRIME_NORMAL                       ,0  ,ModMsgIpcNeedleSampleCmd_PrimeNormal                },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_PRIME_POWER                        ,0  ,ModMsgIpcNeedleSampleCmd_PrimePower                 },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_CLEAN                              ,0  ,ModMsgIpcNeedleSampleCmd_Clean                      },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_CLEAN_POWER                        ,0  ,ModMsgIpcNeedleSampleCmd_CleanPower                 },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_MAINTAIN                           ,0  ,ModMsgIpcNeedleSampleCmd_MainTain                   },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_ROTATE                             ,0  ,ModMsgIpcNeedleSampleCmd_Rotate                     },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_TUBE                 ,0  ,ModMsgIpcNeedleSampleCmd_LiquidAbsorbTube           },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE                ,0  ,ModMsgIpcNeedleSampleCmd_LiquidAbsorbPlate          },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_INJECT_NORMAL               ,0  ,ModMsgIpcNeedleSampleCmd_InjectNormal               },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_INJECT_MAINTAIN             ,0  ,ModMsgIpcNeedleSampleCmd_InjectMaintain             },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST               ,0  ,ModMsgIpcNeedleSampleCmd_LiquidDetectAdjust         },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE           ,0  ,ModMsgIpcNeedleSampleCmd_LiquidDetectReadState      },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_RES_AND_VOL     ,0  ,ModMsgIpcNeedleSampleCmd_LiquidDetectReadResAndVol  },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_DETECT_WRITE_RES            ,0  ,ModMsgIpcNeedleSampleCmd_LiquidDetectWriteRes       },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_BASE_VOL        ,0  ,ModMsgIpcNeedleSampleCmd_LiquidDetectReadBaseVol    },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_READ_NEEDLE_UP_DOWN_EM_STOP_POS    ,0  ,ModMsgIpcNeedleSampleCmd_ReadNeedleUpDownEmStopPos  },
    {IPC_CMD_MACRO_H360_NEEDLE_SAMPLE_READ_LIQUID_BOARD_VERSION          ,0  ,ModMsgIpcNeedleSampleCmd_ReadLiquidBoardVersion     },
};


//IPC指令消息处理函数
void ModMsgIpcNeedleSampleCmd(void* ipcMsgBodyPtr)
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
    uint16_t indexNeedleSample = 0;
    for(indexNeedleSample = 0; indexNeedleSample < DIM_ARRAY_ELEMENT_COUNT(ipcNeedleSampleMsgCmdProcUnitArray);indexNeedleSample++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcNeedleSampleMsgCmdProcUnitArray[indexNeedleSample].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleSample == DIM_ARRAY_ELEMENT_COUNT(ipcNeedleSampleMsgCmdProcUnitArray))
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
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcNeedleSampleMsgCmdProcUnitArray[indexNeedleSample].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcNeedleSampleMsgCmdProcUnitArray[indexNeedleSample].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
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
    if(ipcNeedleSampleMsgCmdProcUnitArray[indexNeedleSample].ipcCmdProcFuncPtr == NULL)
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
    ipcNeedleSampleMsgCmdProcUnitArray[indexNeedleSample].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
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
static void ModMsgIpcNeedleSampleCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
static void ModMsgIpcNeedleSampleCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内输出写入
static void ModMsgIpcNeedleSampleCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机复位
static void ModMsgIpcNeedleSampleCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机走位
static void ModMsgIpcNeedleSampleCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针复位
static void ModMsgIpcNeedleSampleCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针清洗液灌注
static void ModMsgIpcNeedleSampleCmd_PrimeNormal(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针强力清洗液灌注
static void ModMsgIpcNeedleSampleCmd_PrimePower(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针清洗
static void ModMsgIpcNeedleSampleCmd_Clean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针强力清洗
static void ModMsgIpcNeedleSampleCmd_CleanPower(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针清洗维护
static void ModMsgIpcNeedleSampleCmd_MainTain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针旋转指定位置
static void ModMsgIpcNeedleSampleCmd_Rotate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针吸试管样本
static void ModMsgIpcNeedleSampleCmd_LiquidAbsorbTube(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针吸反应盘样本
static void ModMsgIpcNeedleSampleCmd_LiquidAbsorbPlate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针注液
static void ModMsgIpcNeedleSampleCmd_InjectNormal(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针携带维护
static void ModMsgIpcNeedleSampleCmd_InjectMaintain(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//样本针探液校准
static void ModMsgIpcNeedleSampleCmd_LiquidDetectAdjust(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//读样本针探液板状态
static void ModMsgIpcNeedleSampleCmd_LiquidDetectReadState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//读取电阻和针尖电压
static void ModMsgIpcNeedleSampleCmd_LiquidDetectReadResAndVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//写入电阻值
static void ModMsgIpcNeedleSampleCmd_LiquidDetectWriteRes(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//读取针尖基准电压
static void ModMsgIpcNeedleSampleCmd_LiquidDetectReadBaseVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//读样本针升降急停高度
static void ModMsgIpcNeedleSampleCmd_ReadNeedleUpDownEmStopPos(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//读探液板程序版本
static void ModMsgIpcNeedleSampleCmd_ReadLiquidBoardVersion(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}


/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void ModMsgIpcNeedleSampleUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcNeedleSampleMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,ModMsgIpcNeedleSampleUpload_PlaceHolder},
};


//IPC上传消息处理函数
void ModMsgIpcNeedleSampleUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexNeedleSample = 0;
    for(indexNeedleSample = 0; indexNeedleSample < DIM_ARRAY_ELEMENT_COUNT(ipcNeedleSampleMsgUploadProcUnitArray);indexNeedleSample++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcNeedleSampleMsgUploadProcUnitArray[indexNeedleSample].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleSample == DIM_ARRAY_ELEMENT_COUNT(ipcNeedleSampleMsgUploadProcUnitArray))
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
    if(ipcUploadDataPtr->dataLength < ipcNeedleSampleMsgUploadProcUnitArray[indexNeedleSample].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcNeedleSampleMsgUploadProcUnitArray[indexNeedleSample].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
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
    if(ipcNeedleSampleMsgUploadProcUnitArray[indexNeedleSample].ipcUploadProcFuncPtr == NULL)
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
    ipcNeedleSampleMsgUploadProcUnitArray[indexNeedleSample].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void ModMsgIpcNeedleSampleUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/















