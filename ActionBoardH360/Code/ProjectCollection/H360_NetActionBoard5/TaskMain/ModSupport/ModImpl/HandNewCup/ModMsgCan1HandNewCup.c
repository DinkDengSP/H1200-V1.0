/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 16:10:24 +0800
************************************************************************************************/ 
#include "ModImplHandNewCup.h"
#include "ModTaskHeader.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan1HandNewCupCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输入读取
static void ModMsgCan1HandNewCupCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输出写入
static void ModMsgCan1HandNewCupCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机复位
static void ModMsgCan1HandNewCupCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机走位
static void ModMsgCan1HandNewCupCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯机械手复位
static void ModMsgCan1HandNewCupCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯机械手移动到新杯区
static void ModMsgCan1HandNewCupCmd_MoveToNewCupTray(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯机械手移动到反应盘
static void ModMsgCan1HandNewCupCmd_MoveToPlateReaction(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯机械手移动到垃圾桶
static void ModMsgCan1HandNewCupCmd_MoveToGarbage(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯机械手新杯区取杯
static void ModMsgCan1HandNewCupCmd_CatchCupNewTray(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯机械手反应盘取杯
static void ModMsgCan1HandNewCupCmd_CatchCupPlate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯机械手反应盘放杯
static void ModMsgCan1HandNewCupCmd_PutCupPlate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯机械手垃圾桶放杯
static void ModMsgCan1HandNewCupCmd_PutCupGarbage(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcHandNewCupUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                                ,0  ,ENABLE ,ModMsgCan1HandNewCupCmd_PlaceHolder        },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_PORT_IN                   ,1  ,ENABLE ,ModMsgCan1HandNewCupCmd_PortIn             },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_PORT_OUT                  ,2  ,ENABLE ,ModMsgCan1HandNewCupCmd_PortOut            },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_MOTOR_RESET               ,1  ,ENABLE ,ModMsgCan1HandNewCupCmd_MotorReset         },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_MOTOR_RUN_STEPS           ,5  ,ENABLE ,ModMsgCan1HandNewCupCmd_MotorRunSteps      },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_RESET                     ,0  ,ENABLE ,ModMsgCan1HandNewCupCmd_Reset              },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_MOVE_TO_NEW_CUP_TRAY      ,2  ,ENABLE ,ModMsgCan1HandNewCupCmd_MoveToNewCupTray   },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_MOVE_TO_PLATE_REACTION    ,0  ,ENABLE ,ModMsgCan1HandNewCupCmd_MoveToPlateReaction},
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_MOVE_TO_GARBAGE           ,1  ,ENABLE ,ModMsgCan1HandNewCupCmd_MoveToGarbage      },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_CATCH_CUP_NEW_TRAY        ,0  ,ENABLE ,ModMsgCan1HandNewCupCmd_CatchCupNewTray    },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_CATCH_CUP_PLATE           ,0  ,ENABLE ,ModMsgCan1HandNewCupCmd_CatchCupPlate      },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_PUT_CUP_PLATE             ,0  ,ENABLE ,ModMsgCan1HandNewCupCmd_PutCupPlate        },
    {CAN_MASTER_CMD_H360_HAND_NEW_CUP_PUT_CUP_GARBAGE           ,1  ,ENABLE ,ModMsgCan1HandNewCupCmd_PutCupGarbage      },
};


//CAN指令消息处理函数
void ModMsgCan1HandNewCupCmd(void* can1MsgBodyPtr)
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
    uint16_t indexHandNewCup = 0;
    for(indexHandNewCup = 0; indexHandNewCup < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcHandNewCupUnitArray);indexHandNewCup++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcHandNewCupUnitArray[indexHandNewCup].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexHandNewCup == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcHandNewCupUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcHandNewCupUnitArray[indexHandNewCup].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcHandNewCupUnitArray[indexHandNewCup].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcHandNewCupUnitArray[indexHandNewCup].canCmdProcFuncPtr == NULL)
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
    can1CmdProcHandNewCupUnitArray[indexHandNewCup].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcHandNewCupUnitArray[indexHandNewCup].needResultReply == ENABLE)
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
static void ModMsgCan1HandNewCupCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//模块内输入读取
static void ModMsgCan1HandNewCupCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portReadIndex = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,portReadIndex: %d\r\n",__func__,portReadIndex);
    //调用函数实现
    ModImplBaseHandNewCupReadPortIn(portReadIndex,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内输出写入
static void ModMsgCan1HandNewCupCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portWriteIndex = cmdPackPtr->paramBufferPtr[0];
    //获取值
    PIN_STATE setState = (PIN_STATE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,portWriteIndex: %d,setState: %d\r\n",__func__,portWriteIndex,(uint8_t)setState);
    //调用函数实现
    ModImplBaseHandNewCupWritePortOut(portWriteIndex,setState,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机复位
static void ModMsgCan1HandNewCupCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d\r\n",__func__,stepMotorIdx);
    //调用函数实现
    ModImplBaseHandNewCupStepMotorReset(stepMotorIdx,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机走位
static void ModMsgCan1HandNewCupCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    int32_t runSteps = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+1);
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d,runSteps : %d\r\n",__func__,stepMotorIdx,runSteps);
    //调用函数实现
    ModImplBaseHandNewCupStepMotorRunSteps(stepMotorIdx,runSteps,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯机械手复位
static void ModMsgCan1HandNewCupCmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseHandNewCupReset(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯机械手移动到新杯区
static void ModMsgCan1HandNewCupCmd_MoveToNewCupTray(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t colLeftRight = cmdPackPtr->paramBufferPtr[0];
    uint8_t lineBackFront = cmdPackPtr->paramBufferPtr[1];
    //日志显示指令
    SystemPrintf("%s,lineBackFront: %d,colLeftRight: %d\r\n",__func__,lineBackFront,colLeftRight);
    //调用函数实现
    ModImplBaseHandNewCupMoveToNewCupTray(lineBackFront,colLeftRight,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯机械手移动到反应盘
static void ModMsgCan1HandNewCupCmd_MoveToPlateReaction(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseHandNewCupMoveToPlateReaction(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯机械手移动到垃圾桶
static void ModMsgCan1HandNewCupCmd_MoveToGarbage(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    GARBAGE_SELECT garbageNo = (GARBAGE_SELECT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s, garbageNo: %d\r\n",__func__,garbageNo);
    //调用函数实现
    ModImplBaseHandNewCupMoveToGarbage(garbageNo,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯机械手新杯区取杯
static void ModMsgCan1HandNewCupCmd_CatchCupNewTray(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseHandNewCupCatchCupNewTray(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯机械手反应盘取杯
static void ModMsgCan1HandNewCupCmd_CatchCupPlate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseHandNewCupCatchCupPlate(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯机械手反应盘放杯
static void ModMsgCan1HandNewCupCmd_PutCupPlate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseHandNewCupPutCupPlate(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯机械手垃圾桶放杯
static void ModMsgCan1HandNewCupCmd_PutCupGarbage(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    GARBAGE_SELECT garbageNo = (GARBAGE_SELECT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s, garbageNo: %d\r\n",__func__,garbageNo);
    //调用函数实现
    ModImplBaseHandNewCupPutCupGarbage(garbageNo,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}


/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan1HandNewCupUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcHandNewCupUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,ModMsgCan1HandNewCupUpload_PlaceHolder},
};

//CAN上传消息处理函数
void ModMsgCan1HandNewCupUpload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexHandNewCup = 0;
    for(indexHandNewCup = 0; indexHandNewCup < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcHandNewCupUnitArray);indexHandNewCup++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcHandNewCupUnitArray[indexHandNewCup].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexHandNewCup == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcHandNewCupUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcHandNewCupUnitArray[indexHandNewCup].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcHandNewCupUnitArray[indexHandNewCup].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcHandNewCupUnitArray[indexHandNewCup].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcHandNewCupUnitArray[indexHandNewCup].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void ModMsgCan1HandNewCupUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/











