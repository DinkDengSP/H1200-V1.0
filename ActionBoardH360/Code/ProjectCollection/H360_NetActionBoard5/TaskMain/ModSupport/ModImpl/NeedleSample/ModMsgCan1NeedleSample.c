/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-20 15:02:12 +0800
************************************************************************************************/ 
#include "ModImplNeedleSample.h"
#include "ModTaskHeader.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan1NeedleSampleCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输入读取
static void ModMsgCan1NeedleSampleCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输出写入
static void ModMsgCan1NeedleSampleCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机复位
static void ModMsgCan1NeedleSampleCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机走位
static void ModMsgCan1NeedleSampleCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针复位
static void ModMsgCan1NeedleSampleCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针清洗液灌注
static void ModMsgCan1NeedleSampleCmd_PrimeNormal(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针强力清洗液灌注
static void ModMsgCan1NeedleSampleCmd_PrimePower(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针清洗
static void ModMsgCan1NeedleSampleCmd_Clean(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针强力清洗
static void ModMsgCan1NeedleSampleCmd_CleanPower(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针清洗维护
static void ModMsgCan1NeedleSampleCmd_MainTain(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针旋转指定位置
static void ModMsgCan1NeedleSampleCmd_Rotate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针吸试管样本
static void ModMsgCan1NeedleSampleCmd_LiquidAbsorbTube(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针吸反应盘样本
static void ModMsgCan1NeedleSampleCmd_LiquidAbsorbPlate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针注液
static void ModMsgCan1NeedleSampleCmd_InjectNormal(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//样本针探液校准
static void ModMsgCan1NeedleSampleCmd_LiquidDetectAdjust(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//读样本针探液板状态
static void ModMsgCan1NeedleSampleCmd_LiquidDetectReadState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//读取电阻和针尖电压
static void ModMsgCan1NeedleSampleCmd_LiquidDetectReadResAndVol(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//写入电阻值
static void ModMsgCan1NeedleSampleCmd_LiquidDetectWriteRes(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//读取针尖基准电压
static void ModMsgCan1NeedleSampleCmd_LiquidDetectReadBaseVol(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//读样本针升降急停高度
static void ModMsgCan1NeedleSampleCmd_ReadNeedleUpDownEmStopPos(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//读探液板程序版本
static void ModMsgCan1NeedleSampleCmd_ReadLiquidBoardVersion(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcNeedleSampleUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                                          ,0  ,ENABLE ,ModMsgCan1NeedleSampleCmd_PlaceHolder                },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PORT_IN                            ,1  ,ENABLE ,ModMsgCan1NeedleSampleCmd_PortIn                     },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PORT_OUT                           ,2  ,ENABLE ,ModMsgCan1NeedleSampleCmd_PortOut                    },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MOTOR_RESET                        ,1  ,ENABLE ,ModMsgCan1NeedleSampleCmd_MotorReset                 },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MOTOR_RUN_STEPS                    ,5  ,ENABLE ,ModMsgCan1NeedleSampleCmd_MotorRunSteps              },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_RESET                              ,0  ,ENABLE ,ModMsgCan1NeedleSampleCmd_Reset                      },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PRIME_NORMAL                       ,1  ,ENABLE ,ModMsgCan1NeedleSampleCmd_PrimeNormal                },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PRIME_POWER                        ,0  ,ENABLE ,ModMsgCan1NeedleSampleCmd_PrimePower                 },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_CLEAN                              ,2  ,ENABLE ,ModMsgCan1NeedleSampleCmd_Clean                      },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_CLEAN_POWER                        ,1  ,ENABLE ,ModMsgCan1NeedleSampleCmd_CleanPower                 },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MAINTAIN                           ,2  ,ENABLE ,ModMsgCan1NeedleSampleCmd_MainTain                   },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_ROTATE                             ,1  ,ENABLE ,ModMsgCan1NeedleSampleCmd_Rotate                     },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_TUBE                 ,9  ,ENABLE ,ModMsgCan1NeedleSampleCmd_LiquidAbsorbTube           },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE                ,8  ,ENABLE ,ModMsgCan1NeedleSampleCmd_LiquidAbsorbPlate          },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_INJECT_NORMAL               ,5  ,ENABLE ,ModMsgCan1NeedleSampleCmd_InjectNormal               },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST               ,0  ,ENABLE ,ModMsgCan1NeedleSampleCmd_LiquidDetectAdjust         },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE           ,1  ,ENABLE ,ModMsgCan1NeedleSampleCmd_LiquidDetectReadState      },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_RES_AND_VOL     ,0  ,ENABLE ,ModMsgCan1NeedleSampleCmd_LiquidDetectReadResAndVol  },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_WRITE_RES            ,2  ,ENABLE ,ModMsgCan1NeedleSampleCmd_LiquidDetectWriteRes       },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_BASE_VOL        ,0  ,ENABLE ,ModMsgCan1NeedleSampleCmd_LiquidDetectReadBaseVol    },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_READ_NEEDLE_UP_DOWN_EM_STOP_POS    ,0  ,ENABLE ,ModMsgCan1NeedleSampleCmd_ReadNeedleUpDownEmStopPos  },
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_READ_LIQUID_BOARD_VERSION          ,0  ,ENABLE ,ModMsgCan1NeedleSampleCmd_ReadLiquidBoardVersion     },
};


//CAN指令消息处理函数
void ModMsgCan1NeedleSampleCmd(void* can1MsgBodyPtr)
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
    uint16_t indexNeedleSample = 0;
    for(indexNeedleSample = 0; indexNeedleSample < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcNeedleSampleUnitArray);indexNeedleSample++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcNeedleSampleUnitArray[indexNeedleSample].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleSample == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcNeedleSampleUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcNeedleSampleUnitArray[indexNeedleSample].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcNeedleSampleUnitArray[indexNeedleSample].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcNeedleSampleUnitArray[indexNeedleSample].canCmdProcFuncPtr == NULL)
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
    can1CmdProcNeedleSampleUnitArray[indexNeedleSample].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcNeedleSampleUnitArray[indexNeedleSample].needResultReply == ENABLE)
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
static void ModMsgCan1NeedleSampleCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//模块内输入读取
static void ModMsgCan1NeedleSampleCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portReadIndex = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,portReadIndex: %d\r\n",__func__,portReadIndex);
    //调用函数实现
    ModImplBaseNeedleSampleReadPortIn(portReadIndex,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内输出写入
static void ModMsgCan1NeedleSampleCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portWriteIndex = cmdPackPtr->paramBufferPtr[0];
    //获取值
    PIN_STATE setState = (PIN_STATE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,portWriteIndex: %d,setState: %d\r\n",__func__,portWriteIndex,(uint8_t)setState);
    //调用函数实现
    ModImplBaseNeedleSampleWritePortOut(portWriteIndex,setState,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机复位
static void ModMsgCan1NeedleSampleCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d\r\n",__func__,stepMotorIdx);
    //调用函数实现
    ModImplBaseNeedleSampleStepMotorReset(stepMotorIdx,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机走位
static void ModMsgCan1NeedleSampleCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    int32_t runSteps = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+1);
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d,runSteps : %d\r\n",__func__,stepMotorIdx,runSteps);
    //调用函数实现
    ModImplBaseNeedleSampleStepMotorRunSteps(stepMotorIdx,runSteps,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针复位
static void ModMsgCan1NeedleSampleCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseNeedleSampleReset(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针清洗液灌注
static void ModMsgCan1NeedleSampleCmd_PrimeNormal(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_PRIME_OPT opt = (NEEDLE_PRIME_OPT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s, opt: %d\r\n",__func__,opt);
    //调用函数实现
    ModImplBaseNeedleSamplePrimeNormal(opt,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针强力清洗液灌注
static void ModMsgCan1NeedleSampleCmd_PrimePower(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseNeedleSamplePrimePower(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针清洗
static void ModMsgCan1NeedleSampleCmd_Clean(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_CLEAN_OPT opt = (NEEDLE_CLEAN_OPT)(cmdPackPtr->paramBufferPtr[0]);
    NEEDLE_CLEAN_TIME time = (NEEDLE_CLEAN_TIME)(cmdPackPtr->paramBufferPtr[1]);
    //日志显示指令
    SystemPrintf("%s,opt: %d,time: %d\r\n",__func__,opt,time);
    //调用函数实现
    ModImplBaseNeedleSampleClean(opt,time,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针强力清洗
static void ModMsgCan1NeedleSampleCmd_CleanPower(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t count = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,count: %d\r\n",__func__,count);
    //调用函数实现
    ModImplBaseNeedleSampleCleanPower(count,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针清洗维护
static void ModMsgCan1NeedleSampleCmd_MainTain(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_MAINTAIN_OPT opt = (NEEDLE_MAINTAIN_OPT)(cmdPackPtr->paramBufferPtr[0]);
    NEEDLE_CLEAN_TIME time = (NEEDLE_CLEAN_TIME)(cmdPackPtr->paramBufferPtr[1]);
    //日志显示指令
    SystemPrintf("%s,opt: %d,time: %d\r\n",__func__,opt,time);
    //调用函数实现
    ModImplBaseNeedleSampleMainTain(opt,time,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针旋转指定位置
static void ModMsgCan1NeedleSampleCmd_Rotate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    NEEDLE_SAMPLE_POS targetPos = (NEEDLE_SAMPLE_POS)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,targetPos: %d\r\n",__func__,targetPos);
    //调用函数实现
    ModImplBaseNeedleSampleRotate(targetPos,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针吸试管样本
static void ModMsgCan1NeedleSampleCmd_LiquidAbsorbTube(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint32_t unitCount = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr);
    int32_t lastLiquidDetectPos = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+4);
    TUBE_TYPE tubeType = (TUBE_TYPE)(cmdPackPtr->paramBufferPtr[8]);
    //日志显示指令
    SystemPrintf("%s,unitCount: %d,lastLiquidDetectPos: %d,tubeType: %d\r\n",__func__,
                    unitCount,lastLiquidDetectPos,tubeType);
    //调用函数实现
    ModImplBaseNeedleSampleLiquidAbsorbTube(unitCount,lastLiquidDetectPos,tubeType,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针吸反应盘样本
static void ModMsgCan1NeedleSampleCmd_LiquidAbsorbPlate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint32_t unitCount = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr); 
    uint32_t cupLiquidResidueAmp1000 = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+4);
    //日志显示指令
    SystemPrintf("%s,unitCount: %d,cupLiquidResidueAmp1000: %d\r\n",__func__,unitCount,cupLiquidResidueAmp1000);
    //调用函数实现
    ModImplBaseNeedleSampleLiquidAbsorbPlate(unitCount,cupLiquidResidueAmp1000,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针注液
static void ModMsgCan1NeedleSampleCmd_InjectNormal(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint32_t cupLiquidUlAmp1000 = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr); 
    uint8_t mixCount = cmdPackPtr->paramBufferPtr[4];
    //日志显示指令
    SystemPrintf("%s,cupLiquidUlAmp1000: %d,mixCount: %d\r\n",__func__,cupLiquidUlAmp1000,mixCount);
    //调用函数实现
    ModImplBaseNeedleSampleInjectNormal(cupLiquidUlAmp1000,mixCount,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//样本针探液校准
static void ModMsgCan1NeedleSampleCmd_LiquidDetectAdjust(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseNeedleSampleLiquidDetectAdjust(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//读样本针探液板状态
static void ModMsgCan1NeedleSampleCmd_LiquidDetectReadState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    LIQUID_DETECT_CONFIG config = (LIQUID_DETECT_CONFIG)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,config: %d\r\n",__func__,config);
    //调用函数实现
    ModImplBaseNeedleSampleLiquidDetectReadState(config,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//读取电阻和针尖电压
static void ModMsgCan1NeedleSampleCmd_LiquidDetectReadResAndVol(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseNeedleSampleLiquidDetectReadResAndVol(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//写入电阻值
static void ModMsgCan1NeedleSampleCmd_LiquidDetectWriteRes(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint16_t resValue = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr);
    //日志显示指令
    SystemPrintf("%s,resValue: %d\r\n",__func__,resValue);
    //调用函数实现
    ModImplBaseNeedleSampleLiquidDetectWriteRes(resValue,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//读取针尖基准电压
static void ModMsgCan1NeedleSampleCmd_LiquidDetectReadBaseVol(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseNeedleSampleLiquidDetectReadBaseVol(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//读样本针升降急停高度
static void ModMsgCan1NeedleSampleCmd_ReadNeedleUpDownEmStopPos(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseNeedleSampleReadNeedleUpDownEmStopPos(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//读探液板程序版本
static void ModMsgCan1NeedleSampleCmd_ReadLiquidBoardVersion(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseNeedleSampleReadLiquidBoardVersion(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}


/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan1NeedleSampleUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcNeedleSampleUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,ModMsgCan1NeedleSampleUpload_PlaceHolder},
};

//CAN上传消息处理函数
void ModMsgCan1NeedleSampleUpload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexNeedleSample = 0;
    for(indexNeedleSample = 0; indexNeedleSample < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcNeedleSampleUnitArray);indexNeedleSample++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcNeedleSampleUnitArray[indexNeedleSample].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleSample == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcNeedleSampleUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcNeedleSampleUnitArray[indexNeedleSample].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcNeedleSampleUnitArray[indexNeedleSample].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcNeedleSampleUnitArray[indexNeedleSample].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcNeedleSampleUnitArray[indexNeedleSample].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void ModMsgCan1NeedleSampleUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/











