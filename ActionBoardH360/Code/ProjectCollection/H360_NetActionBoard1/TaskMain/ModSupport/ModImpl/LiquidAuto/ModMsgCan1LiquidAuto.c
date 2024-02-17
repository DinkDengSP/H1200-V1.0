/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-28 11:54:32 +0800
************************************************************************************************/ 
#include "ModImplLiquidAuto.h"
#include "ModTaskHeader.h"
#include "ModImplBaseLiquidAuto.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan1LiquidAutoCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输入读取
static void ModMsgCan1LiquidAutoCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输出写入
static void ModMsgCan1LiquidAutoCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机复位
static void ModMsgCan1LiquidAutoCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块被电机走位
static void ModMsgCan1LiquidAutoCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//初始化
static void ModMsgCan1LiquidAutoCmd_Init(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//强制排一次废液
static void ModMsgCan1LiquidAutoCmd_ForceWasteClearOnce(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//自动排废液打开
static void ModMsgCan1LiquidAutoCmd_EnableAutoWasteClear(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//自动排废液关闭
static void ModMsgCan1LiquidAutoCmd_DisableAutoWasteClear(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//查看排废液状态
static void ModMsgCan1LiquidAutoCmd_CheckAutoWasteClearState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//关闭稀释
static void ModMsgCan1LiquidAutoCmd_CloseDilution(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//打开稀释
static void ModMsgCan1LiquidAutoCmd_OpenDilution(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//读取自动稀释纯水脉冲数
static void ModMsgCan1LiquidAutoCmd_GetLiquidDilutionPureWaterPulse(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//清除自动稀释纯水脉冲数
static void ModMsgCan1LiquidAutoCmd_ResetLiquidDilutionPureWaterPulse(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//读取自动稀释纯水触发标记
static void ModMsgCan1LiquidAutoCmd_GetLiquidDilutionPureWaterTrigFlag(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//注册自动稀释纯水脉冲数
static void ModMsgCan1LiquidAutoCmd_RegisterLiquidDilutionPureWaterPulse(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcLiquidAutoUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                                           ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_PlaceHolder                             },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_PORT_IN                               ,1  ,ENABLE ,ModMsgCan1LiquidAutoCmd_PortIn                                  },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_PORT_OUT                              ,2  ,ENABLE ,ModMsgCan1LiquidAutoCmd_PortOut                                 },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_MOTOR_RESET                           ,1  ,ENABLE ,ModMsgCan1LiquidAutoCmd_MotorReset                              },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_MOTOR_RUN_STEPS                       ,5  ,ENABLE ,ModMsgCan1LiquidAutoCmd_MotorRunSteps                           },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_INIT                                  ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_Init                                    },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_FORCE_WASTE_CLR_ONCE                  ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_ForceWasteClearOnce                     },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_EN_AUTO_WASTE_CLR                     ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_EnableAutoWasteClear                    },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_DIS_AUTO_WASTE_CLR                    ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_DisableAutoWasteClear                   },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_CHK_AUTO_WASTE_CLR_STA                ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_CheckAutoWasteClearState                },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_CLOSE_DILUTION                        ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_CloseDilution                           },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_OPEN_DILUTION                         ,1  ,ENABLE ,ModMsgCan1LiquidAutoCmd_OpenDilution                            },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_READ_AUTO_DILUTE_PURE_WATER_PULSE     ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_GetLiquidDilutionPureWaterPulse         },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_RESET_AUTO_DILUTE_PURE_WATER_PULSE    ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_ResetLiquidDilutionPureWaterPulse       },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_READ_AUTO_DILUTE_PURE_WATER_TRIG_FLAG ,0  ,ENABLE ,ModMsgCan1LiquidAutoCmd_GetLiquidDilutionPureWaterTrigFlag      },
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_REGISTER_AUTO_DILUTE_PURE_WATER_PULSE ,4  ,ENABLE ,ModMsgCan1LiquidAutoCmd_RegisterLiquidDilutionPureWaterPulse    },
};


//CAN指令消息处理函数
void ModMsgCan1LiquidAutoCmd(void* can1MsgBodyPtr)
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
    uint16_t indexLiquidAuto = 0;
    for(indexLiquidAuto = 0; indexLiquidAuto < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcLiquidAutoUnitArray);indexLiquidAuto++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcLiquidAutoUnitArray[indexLiquidAuto].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexLiquidAuto == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcLiquidAutoUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcLiquidAutoUnitArray[indexLiquidAuto].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcLiquidAutoUnitArray[indexLiquidAuto].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcLiquidAutoUnitArray[indexLiquidAuto].canCmdProcFuncPtr == NULL)
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
    can1CmdProcLiquidAutoUnitArray[indexLiquidAuto].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcLiquidAutoUnitArray[indexLiquidAuto].needResultReply == ENABLE)
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
static void ModMsgCan1LiquidAutoCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//模块内输入读取
static void ModMsgCan1LiquidAutoCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portReadIndex = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,portReadIndex: %d\r\n",__func__,portReadIndex);
    //调用函数实现
    ModImplBaseLiquidAutoReadPortIn(portReadIndex,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内输出写入
static void ModMsgCan1LiquidAutoCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portWriteIndex = cmdPackPtr->paramBufferPtr[0];
    //获取值
    PIN_STATE setState = (PIN_STATE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,portWriteIndex: %d,setState: %d\r\n",__func__,portWriteIndex,(uint8_t)setState);
    //调用函数实现
    ModImplBaseLiquidAutoWritePortOut(portWriteIndex,setState,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机复位
static void ModMsgCan1LiquidAutoCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d\r\n",__func__,stepMotorIdx);
    //调用函数实现
    ModImplBaseLiquidAutoStepMotorReset(stepMotorIdx,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块被电机走位
static void ModMsgCan1LiquidAutoCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    int32_t runSteps = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+1);
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d,runSteps : %d\r\n",__func__,stepMotorIdx,runSteps);
    //调用函数实现
    ModImplBaseLiquidAutoStepMotorRunSteps(stepMotorIdx,runSteps,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//初始化
static void ModMsgCan1LiquidAutoCmd_Init(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoInit(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//强制排一次废液
static void ModMsgCan1LiquidAutoCmd_ForceWasteClearOnce(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoForceWasteClearOnce(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//自动排废液打开
static void ModMsgCan1LiquidAutoCmd_EnableAutoWasteClear(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoEnableAutoWasteClear(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//自动排废液关闭
static void ModMsgCan1LiquidAutoCmd_DisableAutoWasteClear(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoDisableAutoWasteClear(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//查看排废液状态
static void ModMsgCan1LiquidAutoCmd_CheckAutoWasteClearState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoCheckAutoWasteClearState(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//关闭稀释
static void ModMsgCan1LiquidAutoCmd_CloseDilution(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoCloseDilution(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//打开稀释
static void ModMsgCan1LiquidAutoCmd_OpenDilution(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    DILUTION_CH dilutionSetChannel = (DILUTION_CH)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,dilutionSetChannel: %d\r\n",__func__,dilutionSetChannel);
    //调用函数实现
    ModImplBaseLiquidAutoOpenDilution(dilutionSetChannel,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//读取自动稀释纯水脉冲数
static void ModMsgCan1LiquidAutoCmd_GetLiquidDilutionPureWaterPulse(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoGetLiquidDilutionPureWaterPulse(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//清除自动稀释纯水脉冲数
static void ModMsgCan1LiquidAutoCmd_ResetLiquidDilutionPureWaterPulse(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoResetLiquidDilutionPureWaterPulse(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//读取自动稀释纯水触发标记
static void ModMsgCan1LiquidAutoCmd_GetLiquidDilutionPureWaterTrigFlag(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseLiquidAutoGetLiquidDilutionPureWaterTrigFlag(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//注册自动稀释纯水脉冲数
static void ModMsgCan1LiquidAutoCmd_RegisterLiquidDilutionPureWaterPulse(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint32_t trigCount = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr);
    //日志显示指令
    SystemPrintf("%s,trigCount: %d\r\n",__func__,trigCount);
    //调用函数实现
    ModImplBaseLiquidAutoRegisterLiquidDilutionPureWaterPulse(trigCount,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}


/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan1LiquidAutoUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcLiquidAutoUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,ModMsgCan1LiquidAutoUpload_PlaceHolder},
};

//CAN上传消息处理函数
void ModMsgCan1LiquidAutoUpload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexLiquidAuto = 0;
    for(indexLiquidAuto = 0; indexLiquidAuto < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcLiquidAutoUnitArray);indexLiquidAuto++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcLiquidAutoUnitArray[indexLiquidAuto].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexLiquidAuto == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcLiquidAutoUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcLiquidAutoUnitArray[indexLiquidAuto].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcLiquidAutoUnitArray[indexLiquidAuto].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcLiquidAutoUnitArray[indexLiquidAuto].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcLiquidAutoUnitArray[indexLiquidAuto].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void ModMsgCan1LiquidAutoUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/

