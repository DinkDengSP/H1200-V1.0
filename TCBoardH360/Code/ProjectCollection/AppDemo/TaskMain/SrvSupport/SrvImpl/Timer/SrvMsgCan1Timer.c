/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 11:10:09 +0800
************************************************************************************************/ 
#include "SrvImplTimer.h"
#include "SrvTaskHeader.h"
#include "SrvImplBaseTimer.h"

/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void SrvMsgCan1TimerCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcTimerUnitArray[] = 
{
    //顺序时 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER          ,1  ,ENABLE ,SrvMsgCan1TimerCmd_PlaceHolder    },
};


//CAN指令消息处理函数
void SrvMsgCan1TimerCmd(void* canMsgBodyPtr)
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
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcTimerUnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcTimerUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcTimerUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcTimerUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcTimerUnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcTimerUnitArray[indexUtil].canCmdProcFuncPtr == NULL)
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
    can1CmdProcTimerUnitArray[indexUtil].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcTimerUnitArray[indexUtil].needResultReply == ENABLE)
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
        UserMemFree(MEM_REGION_SRV_TASK,resultDataBufferPtr);
    }
    if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
}

//联机
static void SrvMsgCan1TimerCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    return;
}
/*************************************************************************************************************************************/


/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void SrvMsgCan1TimerUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackRecvPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT canMsgUploadProcessUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,SrvMsgCan1TimerUpload_PlaceHolder},
};

//CAN上传消息处理函数
void SrvMsgCan1TimerUpload(void* canMsgBodyPtr)
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
static void SrvMsgCan1TimerUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackRecvPtr)
{

}
/*************************************************************************************************************************************/












