/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-09 16:40:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-08 13:23:03 +0800
************************************************************************************************/ 
#include "ModImplTempControl.h"
#include "ModImplBaseTempControl.h"
#include "ModTaskHeader.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan2TempControlCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can2CmdProcTempControlUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                  ,0  ,ENABLE ,ModMsgCan2TempControlCmd_PlaceHolder  },
};


//CAN指令消息处理函数
void ModMsgCan2TempControlCmd(void* can2MsgBodyPtr)
{
    //转换指令码
    CAN_PROT_CMD_PACK* cmdPackPtr = (CAN_PROT_CMD_PACK*)can2MsgBodyPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv cmdPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexTempControl = 0;
    for(indexTempControl = 0; indexTempControl < DIM_ARRAY_ELEMENT_COUNT(can2CmdProcTempControlUnitArray);indexTempControl++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can2CmdProcTempControlUnitArray[indexTempControl].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexTempControl == DIM_ARRAY_ELEMENT_COUNT(can2CmdProcTempControlUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can2CmdProcTempControlUnitArray[indexTempControl].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can2CmdProcTempControlUnitArray[indexTempControl].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can2CmdProcTempControlUnitArray[indexTempControl].canCmdProcFuncPtr == NULL)
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
    can2CmdProcTempControlUnitArray[indexTempControl].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can2CmdProcTempControlUnitArray[indexTempControl].needResultReply == ENABLE)
    {
        //需要结果回传
        Can2ProtSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
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
static void ModMsgCan2TempControlCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan2TempControlUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);
//温控上报,反应盘
static void ModMsgCan2TempControlUpload_PlateReaction(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);
//温控上报,清洗盘
static void ModMsgCan2TempControlUpload_PlateWash(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);
//温控上报,试剂盘
static void ModMsgCan2TempControlUpload_PlateReagent(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can2MsgUploadProcTempControlUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE                        ,2 ,ModMsgCan2TempControlUpload_PlaceHolder     },
    {CAN_MASTER_REPORT_H360_PLATE_REACTION_TEMP_CONTROL     ,8 ,ModMsgCan2TempControlUpload_PlateReaction   },
    {CAN_MASTER_REPORT_H360_PLATE_WASH_TEMP_CONTROL         ,8 ,ModMsgCan2TempControlUpload_PlateWash       },
    {CAN_MASTER_REPORT_H360_PLATE_REAGENT_TEMP_CONTROL      ,8 ,ModMsgCan2TempControlUpload_PlateReagent    },
};

//CAN上传消息处理函数
void ModMsgCan2TempControlUpload(void* can2MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can2MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexTempControl = 0;
    for(indexTempControl = 0; indexTempControl < DIM_ARRAY_ELEMENT_COUNT(can2MsgUploadProcTempControlUnitArray);indexTempControl++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can2MsgUploadProcTempControlUnitArray[indexTempControl].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexTempControl == DIM_ARRAY_ELEMENT_COUNT(can2MsgUploadProcTempControlUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can2MsgUploadProcTempControlUnitArray[indexTempControl].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can2MsgUploadProcTempControlUnitArray[indexTempControl].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can2MsgUploadProcTempControlUnitArray[indexTempControl].canUploadProcFuncPtr == NULL)
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
    can2MsgUploadProcTempControlUnitArray[indexTempControl].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void ModMsgCan2TempControlUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{
    asm("nop");
}

//温控上报,反应盘
static void ModMsgCan2TempControlUpload_PlateReaction(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{
    float reportPlateReactionTempVal = (ProtUtilReadInt32Buffer(canUploadPackPtr->uploadDataBufferPtr)/100.0f);
    ERROR_SUB errorSensorState = (ERROR_SUB)(ProtUtilReadUint32Buffer(canUploadPackPtr->uploadDataBufferPtr+4));
    TEMP_CONTROL_STATE controlState = (TEMP_CONTROL_STATE)(canUploadPackPtr->uploadDataBufferPtr[8]);
    //调用事件处理
    ModImplBaseTempControlEventProcPlateReactionReport(reportPlateReactionTempVal,errorSensorState,controlState);
}

//温控上报,清洗盘
static void ModMsgCan2TempControlUpload_PlateWash(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{
    float reportPlateWashTempVal = (ProtUtilReadInt32Buffer(canUploadPackPtr->uploadDataBufferPtr)/100.0f);
    ERROR_SUB errorSensorState = (ERROR_SUB)(ProtUtilReadUint32Buffer(canUploadPackPtr->uploadDataBufferPtr+4));
    TEMP_CONTROL_STATE controlState = (TEMP_CONTROL_STATE)(canUploadPackPtr->uploadDataBufferPtr[8]);
    //调用事件处理
    ModImplBaseTempControlEventProcPlateWashReport(reportPlateWashTempVal,errorSensorState,controlState);
}

//温控上报,试剂盘
static void ModMsgCan2TempControlUpload_PlateReagent(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{
    float reportPlateReagentTempVal = (ProtUtilReadInt32Buffer(canUploadPackPtr->uploadDataBufferPtr)/100.0f);
    ERROR_SUB errorSensorState = (ERROR_SUB)(ProtUtilReadUint32Buffer(canUploadPackPtr->uploadDataBufferPtr+4));
    TEMP_CONTROL_STATE controlState = (TEMP_CONTROL_STATE)(canUploadPackPtr->uploadDataBufferPtr[8]);
    PIN_STATE floatSensorState = (PIN_STATE)(canUploadPackPtr->uploadDataBufferPtr[9]);
    //调用事件处理
    ModImplBaseTempControlEventProcPlateReagentReport(reportPlateReagentTempVal,errorSensorState,controlState,
                                                        floatSensorState);
}

/*************************************************************************************************************************************/
























