/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-20 16:46:39 +0800
************************************************************************************************/ 
#include "ModImplUtil.h"
#include "ModTaskHeader.h"
#include "ModImplBaseUtil.h"

/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan1UtilCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设置温控上报
static void ModMsgCan1UtilCmd_ReportFlagSet(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取板上控温状态
static void ModMsgCan1UtilCmd_GetBoardTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取环境控温状态
static void ModMsgCan1UtilCmd_GetEnvirTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取反应盘控温状态
static void ModMsgCan1UtilCmd_GetPlateReactionTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取清洗盘控温状态
static void ModMsgCan1UtilCmd_GetPlateWashTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取试剂盘控温状态
static void ModMsgCan1UtilCmd_GetPlateReagentTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取试剂盘浮球状态
static void ModMsgCan1UtilCmd_GetPlateReagentFloatSensor(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取反应盘电流检测值
static void ModMsgCan1UtilCmd_GetPlateReactionCurrent(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取清洗盘电流检测值
static void ModMsgCan1UtilCmd_GetPlateWashCurrent(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取试剂盘电流检测值
static void ModMsgCan1UtilCmd_GetPlateReagentCurrent(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);



//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcUtilUnitArray[] = 
{
    //顺序时 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                            ,0  ,ENABLE ,ModMsgCan1UtilCmd_PlaceHolder                  },
    {CAN_MASTER_CMD_H360TC_REPORT_FLAG_SET                  ,2  ,ENABLE ,ModMsgCan1UtilCmd_ReportFlagSet                },
    {CAN_MASTER_CMD_H360TC_GET_BOARD_TEMP_CONTROL           ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetBoardTempControl          },
    {CAN_MASTER_CMD_H360TC_GET_ENVIR_TEMP_CONTROL           ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetEnvirTempControl          },
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REACTION_TEMP_CONTROL  ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetPlateReactionTempControl  },
    {CAN_MASTER_CMD_H360TC_GET_PLATE_WASH_TEMP_CONTROL      ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetPlateWashTempControl      },
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_TEMP_CONTROL   ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetPlateReagentTempControl   },
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_FLOAT_SENSOR   ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetPlateReagentFloatSensor   },
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REACTION_CURRENT       ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetPlateReactionCurrent      },
    {CAN_MASTER_CMD_H360TC_GET_PLATE_WASH_CURRENT           ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetPlateWashCurrent          },
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_CURRENT        ,0  ,ENABLE ,ModMsgCan1UtilCmd_GetPlateReagentCurrent       },
};


//CAN指令消息处理函数
void ModMsgCan1UtilCmd(void* canMsgBodyPtr)
{
    //转换指令码
    CAN_PROT_CMD_PACK* cmdPackPtr = (CAN_PROT_CMD_PACK*)canMsgBodyPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv CanCmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcUtilUnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcUtilUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcUtilUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcUtilUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcUtilUnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcUtilUnitArray[indexUtil].canCmdProcFuncPtr == NULL)
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
    can1CmdProcUtilUnitArray[indexUtil].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcUtilUnitArray[indexUtil].needResultReply == ENABLE)
    {
        //需要结果回传
        Can1ProtSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
    }
    if((resultPack.errorMain != ERROR_MAIN_OK)||(resultPack.errorSub != ERROR_SUB_OK))
    {
        SystemPrintf("E:> %s,%d,Error,Cmd: %d ,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,
                                    cmdPackPtr->cmdValue,resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,
                                    resultPack.errorSub);
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
static void ModMsgCan1UtilCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    return;
}

//设置温控上报
static void ModMsgCan1UtilCmd_ReportFlagSet(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //控制通道
    TC_BASE_CHANNEL tcBaseChannel = (TC_BASE_CHANNEL)(cmdPackPtr->paramBufferPtr[0]);
    //设定状态
    FunctionalState reportFlag = (FunctionalState)(cmdPackPtr->paramBufferPtr[1]);
    uint32_t intervalTimeMs = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+2);
    //日志显示指令
    SystemPrintf("%s,Channel: %d,ReportFlag: %d,IntervalTimeMs\r\n",__func__,(uint8_t)tcBaseChannel,(uint8_t)reportFlag,
                    intervalTimeMs);
    //执行设置温控上报
    ModImplBaseUtilReportFlagSet(tcBaseChannel,reportFlag,intervalTimeMs,cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,
                                    resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取板上控温状态
static void ModMsgCan1UtilCmd_GetBoardTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行获取板上控温状态
    ModImplBaseUtilGetBoardTempControl(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取环境控温状态
static void ModMsgCan1UtilCmd_GetEnvirTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行获取环境控温状态
    ModImplBaseUtilGetEnvirTempControl(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取反应盘控温状态
static void ModMsgCan1UtilCmd_GetPlateReactionTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行 获取反应盘控温状态
    ModImplBaseUtilGetPlateReactionTempControl(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取清洗盘控温状态
static void ModMsgCan1UtilCmd_GetPlateWashTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行 获取清洗盘控温状态
    ModImplBaseUtilGetPlateWashTempControl(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取试剂盘控温状态
static void ModMsgCan1UtilCmd_GetPlateReagentTempControl(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行 获取试剂盘控温状态
    ModImplBaseUtilGetPlateReagentTempControl(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取试剂盘浮球状态
static void ModMsgCan1UtilCmd_GetPlateReagentFloatSensor(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行 获取试剂盘浮球状态
    ModImplBaseUtilGetPlateReagentFloatSensor(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取反应盘电流检测值
static void ModMsgCan1UtilCmd_GetPlateReactionCurrent(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行 获取反应盘电流检测值
    ModImplBaseUtilGetPlateReactionCurrent(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取清洗盘电流检测值
static void ModMsgCan1UtilCmd_GetPlateWashCurrent(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行 获取清洗盘电流检测值
    ModImplBaseUtilGetPlateWashCurrent(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取试剂盘电流检测值
static void ModMsgCan1UtilCmd_GetPlateReagentCurrent(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行 获取试剂盘电流检测值
    ModImplBaseUtilGetPlateReagentCurrent(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}


/*************************************************************************************************************************************/


/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan1UtilUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackRecvPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT canMsgUploadProcessUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,ModMsgCan1UtilUpload_PlaceHolder},
};

//CAN上传消息处理函数
void ModMsgCan1UtilUpload(void* canMsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackRecvPtr = (CAN_PROT_UPLOAD_PACK*)canMsgBodyPtr;
    if(canUploadPackRecvPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackRecvPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(canMsgUploadProcessUnitArray);indexUtil++)
    {
        if(canUploadPackRecvPtr->uploadID == (uint32_t)(canMsgUploadProcessUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(canMsgUploadProcessUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,canUploadPackRecvPtr->uploadID);
        //释放内存
        if((canUploadPackRecvPtr->uploadDataBufferPtr != NULL)&&(canUploadPackRecvPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr);
        return;
    }
    //判定长度
    if(canUploadPackRecvPtr->uploadDataBufferLength < canMsgUploadProcessUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        canMsgUploadProcessUnitArray[indexUtil].exceptParamLengthMin,canUploadPackRecvPtr->uploadDataBufferLength,
                        canUploadPackRecvPtr->uploadID);
        //释放内存
        if((canUploadPackRecvPtr->uploadDataBufferPtr != NULL)&&(canUploadPackRecvPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr);
        return;
    }
    //检测函数指针
    if(canMsgUploadProcessUnitArray[indexUtil].canUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,canUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        canUploadPackRecvPtr->uploadID);
        //释放内存
        if((canUploadPackRecvPtr->uploadDataBufferPtr != NULL)&&(canUploadPackRecvPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr);
        return;
    }
    //调用指令处理
    canMsgUploadProcessUnitArray[indexUtil].canUploadProcFuncPtr(canUploadPackRecvPtr);
    //处理完成,释放内存
    if((canUploadPackRecvPtr->uploadDataBufferPtr != NULL)&&(canUploadPackRecvPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackRecvPtr);
}


//占位函数
static void ModMsgCan1UtilUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackRecvPtr)
{

}
/*************************************************************************************************************************************/

