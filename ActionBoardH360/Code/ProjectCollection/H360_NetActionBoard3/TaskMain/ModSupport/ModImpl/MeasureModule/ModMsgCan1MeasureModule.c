/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-13 10:27:16 +0800
************************************************************************************************/ 
#include "ModImplMeasureModule.h"
#include "ModTaskHeader.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan1MeasureModuleCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输入读取
static void ModMsgCan1MeasureModuleCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输出写入
static void ModMsgCan1MeasureModuleCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机复位
static void ModMsgCan1MeasureModuleCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机走位
static void ModMsgCan1MeasureModuleCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量模块复位
static void ModMsgCan1MeasureModuleCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量上门全开
static void ModMsgCan1MeasureModuleCmd_DoorOpenFull(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量上门半开
static void ModMsgCan1MeasureModuleCmd_DoorOpenHalf(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量上门关闭
static void ModMsgCan1MeasureModuleCmd_DoorClose(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量侧门打开
static void ModMsgCan1MeasureModuleCmd_WindowOpen(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量侧门关闭
static void ModMsgCan1MeasureModuleCmd_WindowClose(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//激发液灌注
static void ModMsgCan1MeasureModuleCmd_PrimeOnce(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//激发液注液
static void ModMsgCan1MeasureModuleCmd_InjectOnce(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量模块清洗维护
static void ModMsgCan1MeasureModuleCmd_InjectMaintain(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量模块暗计数
static void ModMsgCan1MeasureModuleCmd_ReadDark(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量模块读本底
static void ModMsgCan1MeasureModuleCmd_BackGround(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量模块注液测光
static void ModMsgCan1MeasureModuleCmd_ReadLightInject(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量模块集成单次测量
static void ModMsgCan1MeasureModuleCmd_ReadLightComplex(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//测量模块读光子变化率
static void ModMsgCan1MeasureModuleCmd_ReadLightRate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcMeasureModuleUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                          ,0  ,ENABLE ,ModMsgCan1MeasureModuleCmd_PlaceHolder       },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_PORT_IN           ,1  ,ENABLE ,ModMsgCan1MeasureModuleCmd_PortIn            },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_PORT_OUT          ,2  ,ENABLE ,ModMsgCan1MeasureModuleCmd_PortOut           },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_MOTOR_RESET       ,1  ,ENABLE ,ModMsgCan1MeasureModuleCmd_MotorReset        },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_MOTOR_RUN_STEPS   ,5  ,ENABLE ,ModMsgCan1MeasureModuleCmd_MotorRunSteps     },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_RESET             ,1  ,ENABLE ,ModMsgCan1MeasureModuleCmd_Reset             },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_OPEN_FULL    ,0  ,ENABLE ,ModMsgCan1MeasureModuleCmd_DoorOpenFull      },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_OPEN_HALF    ,0  ,ENABLE ,ModMsgCan1MeasureModuleCmd_DoorOpenHalf      },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_CLOSE        ,0  ,ENABLE ,ModMsgCan1MeasureModuleCmd_DoorClose         },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_WINDOW_OPEN       ,0  ,ENABLE ,ModMsgCan1MeasureModuleCmd_WindowOpen        },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_WINDOW_CLOSE      ,0  ,ENABLE ,ModMsgCan1MeasureModuleCmd_WindowClose       },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_PRIME_ONCE        ,1  ,ENABLE ,ModMsgCan1MeasureModuleCmd_PrimeOnce         },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_INJECT_ONCE       ,1  ,ENABLE ,ModMsgCan1MeasureModuleCmd_InjectOnce        },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_INJECT_MAINTAIN   ,1  ,ENABLE ,ModMsgCan1MeasureModuleCmd_InjectMaintain    },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_DARK         ,0  ,ENABLE ,ModMsgCan1MeasureModuleCmd_ReadDark          },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_BACK_GROUND       ,0  ,ENABLE ,ModMsgCan1MeasureModuleCmd_BackGround        },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_INJECT ,1  ,ENABLE ,ModMsgCan1MeasureModuleCmd_ReadLightInject   },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_COMPLEX,2  ,ENABLE ,ModMsgCan1MeasureModuleCmd_ReadLightComplex  },
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_RATE   ,2  ,ENABLE ,ModMsgCan1MeasureModuleCmd_ReadLightRate     },
};


//CAN指令消息处理函数
void ModMsgCan1MeasureModuleCmd(void* can1MsgBodyPtr)
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
    uint16_t indexMeasureModule = 0;
    for(indexMeasureModule = 0; indexMeasureModule < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcMeasureModuleUnitArray);indexMeasureModule++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcMeasureModuleUnitArray[indexMeasureModule].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexMeasureModule == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcMeasureModuleUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcMeasureModuleUnitArray[indexMeasureModule].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcMeasureModuleUnitArray[indexMeasureModule].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcMeasureModuleUnitArray[indexMeasureModule].canCmdProcFuncPtr == NULL)
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
    can1CmdProcMeasureModuleUnitArray[indexMeasureModule].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcMeasureModuleUnitArray[indexMeasureModule].needResultReply == ENABLE)
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
static void ModMsgCan1MeasureModuleCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//模块内输入读取
static void ModMsgCan1MeasureModuleCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portReadIndex = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,portReadIndex: %d\r\n",__func__,portReadIndex);
    //调用函数实现
    ModImplBaseMeasureModuleReadPortIn(portReadIndex,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内输出写入
static void ModMsgCan1MeasureModuleCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portWriteIndex = cmdPackPtr->paramBufferPtr[0];
    //获取值
    PIN_STATE setState = (PIN_STATE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,portWriteIndex: %d,setState: %d\r\n",__func__,portWriteIndex,(uint8_t)setState);
    //调用函数实现
    ModImplBaseMeasureModuleWritePortOut(portWriteIndex,setState,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机复位
static void ModMsgCan1MeasureModuleCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d\r\n",__func__,stepMotorIdx);
    //调用函数实现
    ModImplBaseMeasureModuleStepMotorReset(stepMotorIdx,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机走位
static void ModMsgCan1MeasureModuleCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    int32_t runSteps = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+1);
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d,runSteps : %d\r\n",__func__,stepMotorIdx,runSteps);
    //调用函数实现
    ModImplBaseMeasureModuleStepMotorRunSteps(stepMotorIdx,runSteps,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量模块复位
static void ModMsgCan1MeasureModuleCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOTTLE_SELECT_MEASURE_MODULE bottle = (BOTTLE_SELECT_MEASURE_MODULE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,bottle: %d\r\n",__func__,bottle);
    //调用函数实现
    ModImplBaseMeasureModuleReset(bottle,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量上门全开
static void ModMsgCan1MeasureModuleCmd_DoorOpenFull(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMeasureModuleDoorOpenFull(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量上门半开
static void ModMsgCan1MeasureModuleCmd_DoorOpenHalf(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMeasureModuleDoorOpenHalf(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量上门关闭
static void ModMsgCan1MeasureModuleCmd_DoorClose(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMeasureModuleDoorClose(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量侧门打开
static void ModMsgCan1MeasureModuleCmd_WindowOpen(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMeasureModuleWindowOpen(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量侧门关闭
static void ModMsgCan1MeasureModuleCmd_WindowClose(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMeasureModuleWindowClose(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//激发液灌注
static void ModMsgCan1MeasureModuleCmd_PrimeOnce(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOTTLE_SELECT_MEASURE_MODULE bottle= (BOTTLE_SELECT_MEASURE_MODULE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,bottle: %d\r\n",__func__,bottle);
    //调用函数实现
    ModImplBaseMeasureModulePrimeOnce(bottle,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//激发液注液
static void ModMsgCan1MeasureModuleCmd_InjectOnce(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOTTLE_SELECT_MEASURE_MODULE bottle= (BOTTLE_SELECT_MEASURE_MODULE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,bottle: %d\r\n",__func__,bottle);
    //调用函数实现
    ModImplBaseMeasureModuleInjectOnce(bottle,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量模块清洗维护
static void ModMsgCan1MeasureModuleCmd_InjectMaintain(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOTTLE_SELECT_MEASURE_MODULE bottle= (BOTTLE_SELECT_MEASURE_MODULE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,bottle: %d\r\n",__func__,bottle);
    //调用函数实现
    ModImplBaseMeasureModuleInjectMaintain(bottle,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量模块暗计数
static void ModMsgCan1MeasureModuleCmd_ReadDark(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMeasureModuleReadDark(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量模块读本底
static void ModMsgCan1MeasureModuleCmd_BackGround(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMeasureModuleBackGround(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量模块注液测光
static void ModMsgCan1MeasureModuleCmd_ReadLightInject(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOTTLE_SELECT_MEASURE_MODULE bottle= (BOTTLE_SELECT_MEASURE_MODULE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,bottle: %d\r\n",__func__,bottle);
    //调用函数实现
    ModImplBaseMeasureModuleReadLightInject(bottle,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量模块集成单次测量
static void ModMsgCan1MeasureModuleCmd_ReadLightComplex(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOTTLE_SELECT_MEASURE_MODULE bottle = (BOTTLE_SELECT_MEASURE_MODULE)(cmdPackPtr->paramBufferPtr[0]);
    FunctionalState enableInject = (FunctionalState)(cmdPackPtr->paramBufferPtr[1]);
    //日志显示指令
    SystemPrintf("%s,bottle: %d,enableInject: %d\r\n",__func__,bottle,enableInject);
    //调用函数实现
    ModImplBaseMeasureModuleReadLightComplex(bottle,enableInject,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//测量模块读光子变化率
static void ModMsgCan1MeasureModuleCmd_ReadLightRate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOTTLE_SELECT_MEASURE_MODULE bottle = (BOTTLE_SELECT_MEASURE_MODULE)(cmdPackPtr->paramBufferPtr[0]);;
    uint8_t setChangeCount = cmdPackPtr->paramBufferPtr[1];
    //日志显示指令
    SystemPrintf("%s,bottle: %d,setChangeCount: %d\r\n",__func__,bottle,setChangeCount);
    //调用函数实现
    ModImplBaseMeasureModuleReadLightRate(bottle,setChangeCount,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}


/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan1MeasureModuleUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcMeasureModuleUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,ModMsgCan1MeasureModuleUpload_PlaceHolder},
};

//CAN上传消息处理函数
void ModMsgCan1MeasureModuleUpload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexMeasureModule = 0;
    for(indexMeasureModule = 0; indexMeasureModule < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcMeasureModuleUnitArray);indexMeasureModule++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcMeasureModuleUnitArray[indexMeasureModule].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexMeasureModule == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcMeasureModuleUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcMeasureModuleUnitArray[indexMeasureModule].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcMeasureModuleUnitArray[indexMeasureModule].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcMeasureModuleUnitArray[indexMeasureModule].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcMeasureModuleUnitArray[indexMeasureModule].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void ModMsgCan1MeasureModuleUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/











