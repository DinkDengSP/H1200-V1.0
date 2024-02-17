/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-02 16:40:27 +0800
************************************************************************************************/ 
#include "ModImplLiquidNeedleReagent.h"
#include "ModTaskHeader.h"
#include "ModImplBaseLiquidNeedleReagent.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan1LiquidNeedleReagentCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输入读取
static void ModMsgCan1LiquidNeedleReagentCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输出写入
static void ModMsgCan1LiquidNeedleReagentCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机复位
static void ModMsgCan1LiquidNeedleReagentCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机走位
static void ModMsgCan1LiquidNeedleReagentCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//复位
static void ModMsgCan1LiquidNeedleReagentCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//打开清洗液路
static void ModMsgCan1LiquidNeedleReagentCmd_OpenClean(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//关闭清洗液路
static void ModMsgCan1LiquidNeedleReagentCmd_CloseClean(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//打开灌注液路
static void ModMsgCan1LiquidNeedleReagentCmd_OpenPrime(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//关闭灌注液路
static void ModMsgCan1LiquidNeedleReagentCmd_ClosePrime(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//打开清洗维护液路
static void ModMsgCan1LiquidNeedleReagentCmd_OpenMaintain(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//关闭清洗维护液路
static void ModMsgCan1LiquidNeedleReagentCmd_CloseMaintain(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcLiquidNeedleReagentUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                              ,0  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_PlaceHolder  },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_PORT_IN           ,1  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_PortIn       },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_PORT_OUT          ,2  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_PortOut      },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_MOTOR_RESET       ,1  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_MotorReset   },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_MOTOR_RUN_STEPS   ,5  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_MotorRunSteps},
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_RESET             ,0  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_Reset        },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_OPEN_CLEAN        ,1  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_OpenClean    },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_CLOSE_CLEAN       ,1  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_CloseClean   },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_OPEN_PRIME        ,1  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_OpenPrime    },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_CLOSE_PRIME       ,1  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_ClosePrime   },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_OPEN_MAINTAIN     ,1  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_OpenMaintain },
    {CAN_MASTER_CMD_H360_LIQUID_NDL_REAGENT_CLOSE_MAINTAIN    ,1  ,ENABLE ,ModMsgCan1LiquidNeedleReagentCmd_CloseMaintain},
};


//CAN指令消息处理函数
void ModMsgCan1LiquidNeedleReagentCmd(void* can1MsgBodyPtr)
{
    //转换指令码
    CAN_PROT_CMD_PACK* cmdPackPtr = (CAN_PROT_CMD_PACK*)can1MsgBodyPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv cmdPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexLiquidNeedleReagent = 0;
    for(indexLiquidNeedleReagent = 0; indexLiquidNeedleReagent < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcLiquidNeedleReagentUnitArray);indexLiquidNeedleReagent++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexLiquidNeedleReagent == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcLiquidNeedleReagentUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
        return;
    }
    //判定长度
    if(cmdPackPtr->paramBufferLength < can1CmdProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
                        cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
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
    if(can1CmdProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].canCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,canCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
        return;
    }
    //调用处理函数
    can1CmdProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].needResultReply == ENABLE)
    {
        //需要结果回传
        Can1ProtSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
    }
    //释放内存
    if((resultDataBufferPtr != NULL)&&(resultDataLength != 0))
    {
        UserMemFree(MEM_REGION_MOD_TASK,resultDataBufferPtr);
    }
    if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
}

//占位函数
static void ModMsgCan1LiquidNeedleReagentCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//模块内输入读取
static void ModMsgCan1LiquidNeedleReagentCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portReadIndex = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,portReadIndex: %d\r\n",__func__,portReadIndex);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentReadPortIn(portReadIndex,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内输出写入
static void ModMsgCan1LiquidNeedleReagentCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portWriteIndex = cmdPackPtr->paramBufferPtr[0];
    //获取值
    PIN_STATE setState = (PIN_STATE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,portWriteIndex: %d,setState: %d\r\n",__func__,portWriteIndex,(uint8_t)setState);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentWritePortOut(portWriteIndex,setState,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机复位
static void ModMsgCan1LiquidNeedleReagentCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d\r\n",__func__,stepMotorIdx);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentStepMotorReset(stepMotorIdx,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机走位
static void ModMsgCan1LiquidNeedleReagentCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    int32_t runSteps = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+1);
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d,runSteps : %d\r\n",__func__,stepMotorIdx,runSteps);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentStepMotorRunSteps(stepMotorIdx,runSteps,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//复位
static void ModMsgCan1LiquidNeedleReagentCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentReset(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//打开清洗液路
static void ModMsgCan1LiquidNeedleReagentCmd_OpenClean(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_CLEAN_OPT cleanOpt = (NEEDLE_CLEAN_OPT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,cleanOpt : %d\r\n",__func__,cleanOpt);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentOpenClean(cleanOpt,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//关闭清洗液路
static void ModMsgCan1LiquidNeedleReagentCmd_CloseClean(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_CLEAN_OPT cleanOpt = (NEEDLE_CLEAN_OPT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,cleanOpt : %d\r\n",__func__,cleanOpt);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentCloseClean(cleanOpt,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//打开灌注液路
static void ModMsgCan1LiquidNeedleReagentCmd_OpenPrime(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_PRIME_OPT primeOpt = (NEEDLE_PRIME_OPT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,primeOpt : %d\r\n",__func__,primeOpt);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentOpenPrime(primeOpt,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//关闭灌注液路
static void ModMsgCan1LiquidNeedleReagentCmd_ClosePrime(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_PRIME_OPT primeOpt = (NEEDLE_PRIME_OPT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,primeOpt : %d\r\n",__func__,primeOpt);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentClosePrime(primeOpt,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//打开清洗维护液路
static void ModMsgCan1LiquidNeedleReagentCmd_OpenMaintain(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_MAINTAIN_OPT maintainOpt = (NEEDLE_MAINTAIN_OPT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,maintainOpt : %d\r\n",__func__,maintainOpt);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentOpenMaintain(maintainOpt,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//关闭清洗维护液路
static void ModMsgCan1LiquidNeedleReagentCmd_CloseMaintain(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_MAINTAIN_OPT maintainOpt = (NEEDLE_MAINTAIN_OPT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,maintainOpt : %d\r\n",__func__,maintainOpt);
    //调用函数实现
    ModImplBaseLiquidNeedleReagentCloseMaintain(maintainOpt,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}



/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan1LiquidNeedleReagentUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcLiquidNeedleReagentUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,ModMsgCan1LiquidNeedleReagentUpload_PlaceHolder},
};

//CAN上传消息处理函数
void ModMsgCan1LiquidNeedleReagentUpload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexLiquidNeedleReagent = 0;
    for(indexLiquidNeedleReagent = 0; indexLiquidNeedleReagent < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcLiquidNeedleReagentUnitArray);indexLiquidNeedleReagent++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexLiquidNeedleReagent == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcLiquidNeedleReagentUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //判定长度
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
                        canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //检测函数指针
    if(can1MsgUploadProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].canUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,canUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //调用指令处理
    can1MsgUploadProcLiquidNeedleReagentUnitArray[indexLiquidNeedleReagent].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void ModMsgCan1LiquidNeedleReagentUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/

