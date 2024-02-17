/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 13:34:52 +0800
************************************************************************************************/ 
#include "ModImplMixBead.h"
#include "ModTaskHeader.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan1MixBeadCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输入读取
static void ModMsgCan1MixBeadCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输出写入
static void ModMsgCan1MixBeadCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机复位
static void ModMsgCan1MixBeadCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机走位
static void ModMsgCan1MixBeadCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//复位
static void ModMsgCan1MixBeadCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//磁珠混匀升起来
static void ModMsgCan1MixBeadCmd_Up(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//磁珠混匀降下去
static void ModMsgCan1MixBeadCmd_Down(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//磁珠混匀转一次
static void ModMsgCan1MixBeadCmd_StartRotate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//集成磁珠混匀
static void ModMsgCan1MixBeadCmd_RotateIntegrated(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcMixBeadUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                        ,0  ,ENABLE ,ModMsgCan1MixBeadCmd_PlaceHolder       },
    {CAN_MASTER_CMD_H360_MIX_BEAD_PORT_IN               ,1  ,ENABLE ,ModMsgCan1MixBeadCmd_PortIn            },
    {CAN_MASTER_CMD_H360_MIX_BEAD_PORT_OUT              ,2  ,ENABLE ,ModMsgCan1MixBeadCmd_PortOut           },
    {CAN_MASTER_CMD_H360_MIX_BEAD_MOTOR_RESET           ,1  ,ENABLE ,ModMsgCan1MixBeadCmd_MotorReset        },
    {CAN_MASTER_CMD_H360_MIX_BEAD_MOTOR_RUN_STEPS       ,5  ,ENABLE ,ModMsgCan1MixBeadCmd_MotorRunSteps     },
    {CAN_MASTER_CMD_H360_MIX_BEAD_RESET                 ,0  ,ENABLE ,ModMsgCan1MixBeadCmd_Reset             },
    {CAN_MASTER_CMD_H360_MIX_BEAD_UP                    ,1  ,ENABLE ,ModMsgCan1MixBeadCmd_Up                },
    {CAN_MASTER_CMD_H360_MIX_BEAD_DOWN                  ,0  ,ENABLE ,ModMsgCan1MixBeadCmd_Down              },
    {CAN_MASTER_CMD_H360_MIX_BEAD_START_ROTATE          ,1  ,ENABLE ,ModMsgCan1MixBeadCmd_StartRotate       },
    {CAN_MASTER_CMD_H360_MIX_BEAD_ROTATE_INTEGRATED    ,1  ,ENABLE ,ModMsgCan1MixBeadCmd_RotateIntegrated  },
};


//CAN指令消息处理函数
void ModMsgCan1MixBeadCmd(void* can1MsgBodyPtr)
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
    uint16_t indexMixBead = 0;
    for(indexMixBead = 0; indexMixBead < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcMixBeadUnitArray);indexMixBead++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcMixBeadUnitArray[indexMixBead].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexMixBead == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcMixBeadUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcMixBeadUnitArray[indexMixBead].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcMixBeadUnitArray[indexMixBead].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcMixBeadUnitArray[indexMixBead].canCmdProcFuncPtr == NULL)
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
    can1CmdProcMixBeadUnitArray[indexMixBead].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcMixBeadUnitArray[indexMixBead].needResultReply == ENABLE)
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
static void ModMsgCan1MixBeadCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//模块内输入读取
static void ModMsgCan1MixBeadCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portReadIndex = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,portReadIndex: %d\r\n",__func__,portReadIndex);
    //调用函数实现
    ModImplBaseMixBeadReadPortIn(portReadIndex,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内输出写入
static void ModMsgCan1MixBeadCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portWriteIndex = cmdPackPtr->paramBufferPtr[0];
    //获取值
    PIN_STATE setState = (PIN_STATE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,portWriteIndex: %d,setState: %d\r\n",__func__,portWriteIndex,(uint8_t)setState);
    //调用函数实现
    ModImplBaseMixBeadWritePortOut(portWriteIndex,setState,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机复位
static void ModMsgCan1MixBeadCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d\r\n",__func__,stepMotorIdx);
    //调用函数实现
    ModImplBaseMixBeadStepMotorReset(stepMotorIdx,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机走位
static void ModMsgCan1MixBeadCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    int32_t runSteps = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+1);
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d,runSteps : %d\r\n",__func__,stepMotorIdx,runSteps);
    //调用函数实现
    ModImplBaseMixBeadStepMotorRunSteps(stepMotorIdx,runSteps,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//复位
static void ModMsgCan1MixBeadCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMixBeadReset(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//清洗混匀升起来
static void ModMsgCan1MixBeadCmd_Up(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    MIX_MODE mixMode = (MIX_MODE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,mixMode: %d\r\n",__func__,mixMode);
    //调用函数实现
    ModImplBaseMixBeadUp(mixMode,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//清洗混匀降下去
static void ModMsgCan1MixBeadCmd_Down(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseMixBeadDown(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//清洗混匀转一次
static void ModMsgCan1MixBeadCmd_StartRotate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    MIX_MODE mixMode = (MIX_MODE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,mixMode: %d\r\n",__func__,mixMode);
    //调用函数实现
    ModImplBaseMixBeadStartRotate(mixMode,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//集成清洗混匀
static void ModMsgCan1MixBeadCmd_RotateIntegrated(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    MIX_MODE mixMode = (MIX_MODE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,mixMode: %d\r\n",__func__,mixMode);
    //调用函数实现
    ModImplBaseMixBeadRotateIntegrated(mixMode,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}


/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan1MixBeadUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcMixBeadUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,ModMsgCan1MixBeadUpload_PlaceHolder},
};

//CAN上传消息处理函数
void ModMsgCan1MixBeadUpload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexMixBead = 0;
    for(indexMixBead = 0; indexMixBead < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcMixBeadUnitArray);indexMixBead++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcMixBeadUnitArray[indexMixBead].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexMixBead == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcMixBeadUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcMixBeadUnitArray[indexMixBead].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcMixBeadUnitArray[indexMixBead].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcMixBeadUnitArray[indexMixBead].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcMixBeadUnitArray[indexMixBead].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void ModMsgCan1MixBeadUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/





















