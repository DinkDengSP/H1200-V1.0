/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#include "SrvImplUtil.h"
#include "CommonSrvTaskHeader.h"
#include "SrvImplBaseUtil.h"

/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void SrvMsgCan1UtilCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//联机
static void SrvMsgCan1UtilCmd_OnLine(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//脱机
static void SrvMsgCan1UtilCmd_OffLine(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//UTC同步
static void SrvMsgCan1UtilCmd_UtcSync(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//心跳获取
static void SrvMsgCan1UtilCmd_HeartGet(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//急停
static void SrvMsgCan1UtilCmd_EmStop(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取版本号
static void SrvMsgCan1UtilCmd_GetVersion(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcUtilUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER         ,0  ,ENABLE ,SrvMsgCan1UtilCmd_PlaceHolder },
    {CAN_MASTER_CMD_UTIL_ONLINE          ,1  ,ENABLE ,SrvMsgCan1UtilCmd_OnLine      },
    {CAN_MASTER_CMD_UTIL_OFFLINE         ,1  ,ENABLE ,SrvMsgCan1UtilCmd_OffLine     },
    {CAN_MASTER_CMD_UTIL_UTC_SYNC        ,8  ,ENABLE ,SrvMsgCan1UtilCmd_UtcSync     },
    {CAN_MASTER_CMD_UTIL_HEART_GET       ,0  ,ENABLE ,SrvMsgCan1UtilCmd_HeartGet    },
    {CAN_MASTER_CMD_UTIL_EM_STOP         ,0  ,ENABLE ,SrvMsgCan1UtilCmd_EmStop      },
    {CAN_MASTER_CMD_UTIL_GET_VERSION     ,0  ,ENABLE ,SrvMsgCan1UtilCmd_GetVersion  },
};

//占位函数
static void SrvMsgCan1ParamCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//单个参数读取
static void SrvMsgCan1ParamReadWriteCmd_ReadSingle(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//批量参数读取
static void SrvMsgCan1ParamReadWriteCmd_ReadAny(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//单个参数写入
static void SrvMsgCan1ParamReadWriteCmd_WriteSingle(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//批量参数写入
static void SrvMsgCan1ParamReadWriteCmd_WriteAny(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcParamUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER             ,0  ,ENABLE ,SrvMsgCan1ParamCmd_PlaceHolder          },
    {CAN_MASTER_CMD_PARAM_RW_READ_SINGLE     ,2  ,ENABLE ,SrvMsgCan1ParamReadWriteCmd_ReadSingle  },
    {CAN_MASTER_CMD_PARAM_RW_READ_ANY        ,2  ,ENABLE ,SrvMsgCan1ParamReadWriteCmd_ReadAny     },
    {CAN_MASTER_CMD_PARAM_RW_WRITE_SINGLE    ,6  ,ENABLE ,SrvMsgCan1ParamReadWriteCmd_WriteSingle },
    {CAN_MASTER_CMD_PARAM_RW_WRITE_ANY       ,6  ,ENABLE ,SrvMsgCan1ParamReadWriteCmd_WriteAny    },
};


//CAN指令消息处理函数
void SrvMsgCan1UtilCmd(void* can1MsgBodyPtr)
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
    uint16_t indexUtil = 0;
    //校验通过执行代码
    SYS_RESULT_PACK resultPack;
    uint8_t* resultDataBufferPtr = NULL;
    uint16_t resultDataLength = 0;
    //初始化错误返回包
    resultPack.resultCode = PROT_RESULT_FAIL;
    resultPack.errorMain = ERROR_MAIN_OK;
    resultPack.errorLevel = ERROR_LEVEL_NONE;
    resultPack.errorSub = ERROR_SUB_OK;
    if(cmdPackPtr->singalChannel == CAN_MASTER_CH_ACT_UTIL)
    {
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
    else if(cmdPackPtr->singalChannel == CAN_MASTER_CH_ACT_PARAM_RW)
    {
        for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcParamUnitArray);indexUtil++)
        {
            if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcParamUnitArray[indexUtil].commandCode))
            {
                break;
            }
        }
        //判定序号
        if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcParamUnitArray))
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
        if(cmdPackPtr->paramBufferLength < can1CmdProcParamUnitArray[indexUtil].exceptParamLengthMin)
        {
            //小于最小指令预期长度
            SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                            can1CmdProcParamUnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
                            cmdPackPtr->cmdValue);
            //释放内存
            if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
            {
                UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
            }
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
            return;
        }
        //检测函数指针
        if(can1CmdProcParamUnitArray[indexUtil].canCmdProcFuncPtr == NULL)
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
        can1CmdProcParamUnitArray[indexUtil].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
        //结果处理,是否需要结果回传
        if(can1CmdProcParamUnitArray[indexUtil].needResultReply == ENABLE)
        {
            //需要结果回传
            Can1ProtSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
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
}

//占位函数
static void SrvMsgCan1UtilCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//联机
static void SrvMsgCan1UtilCmd_OnLine(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //获取联机模式
    PROT_CONNECT_FLAG protConnectFlag = (PROT_CONNECT_FLAG)(cmdPackPtr->paramBufferPtr[0]);
    //执行联机
    SrvImplBaseUtilOnline(protConnectFlag,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//脱机
static void SrvMsgCan1UtilCmd_OffLine(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    PROT_DISCONNECT_FLAG disconnectFlag = (PROT_DISCONNECT_FLAG)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,disconnectFlag: %d\r\n",__func__,(uint8_t)disconnectFlag);
    //执行脱机
    SrvImplBaseUtilOffLine(disconnectFlag,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//UTC同步
static void SrvMsgCan1UtilCmd_UtcSync(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint64_t utcTimeStamp = ProtUtilReadUint64Buffer(cmdPackPtr->paramBufferPtr);
    //转换UTC时间
    utcTimeStamp /= 1000;
    //加上东八区时间
    utcTimeStamp += (8*60*60);
    //日志显示指令
    SystemPrintf("%s,utcTimeStamp: %lld\r\n",__func__,utcTimeStamp);
    //执行同步
    SrvImplBaseUtilUtcSync(utcTimeStamp,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//心跳获取
static void SrvMsgCan1UtilCmd_HeartGet(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行心跳获取
    SrvImplBaseUtilHeartGet(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//急停
static void SrvMsgCan1UtilCmd_EmStop(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行急停
    SrvImplBaseUtilEmStop(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取版本号
static void SrvMsgCan1UtilCmd_GetVersion(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //执行版本号获取
    SrvImplBaseUtilGetVersion(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//占位函数
static void SrvMsgCan1ParamCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//单个参数读取
static void SrvMsgCan1ParamReadWriteCmd_ReadSingle(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t mainIndex = cmdPackPtr->paramBufferPtr[0];
    uint8_t subIndex = cmdPackPtr->paramBufferPtr[1];
    //日志参数打印
    SystemPrintf("%s,mainIndex : %d,subIndex : %d\r\n",__func__,mainIndex,subIndex);
    //调用实现
    SrvImplBaseUtilParamReadSingle(mainIndex,subIndex,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//批量参数读取
static void SrvMsgCan1ParamReadWriteCmd_ReadAny(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t readParamPair = cmdPackPtr->paramBufferLength/2;
    //日志参数打印
    SystemPrintf("%s,readParamPair : %d\r\n",__func__,readParamPair);
    uint8_t* readParaPairDataArrayPtr = cmdPackPtr->paramBufferPtr;
    //执行参数批量读取
    SrvImplBaseUtilParamReadAny(readParamPair,readParaPairDataArrayPtr,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//单个参数写入
static void SrvMsgCan1ParamReadWriteCmd_WriteSingle(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t mainIndex = cmdPackPtr->paramBufferPtr[0];
    uint8_t subIndex = cmdPackPtr->paramBufferPtr[1];
    int32_t writeParam = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+2);
    //日志参数打印
    SystemPrintf("%s,writeParamSingle MainIndex: %d,SubIndex: %d,Value: %d\r\n",__func__,mainIndex,subIndex,writeParam);
    //执行参数单个写入
    SrvImplBaseUtilParamWriteSingle(mainIndex,subIndex,writeParam,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//批量参数写入
static void SrvMsgCan1ParamReadWriteCmd_WriteAny(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t writeParamPair = cmdPackPtr->paramBufferLength/6;
    //日志参数打印
    SystemPrintf("%s,writeParamPair : %d\r\n",__func__,writeParamPair);
    uint8_t* writeParamPairArrayPtr = cmdPackPtr->paramBufferPtr;
    //执行参数批量写入
    SrvImplBaseUtilParamWriteAny(writeParamPair,writeParamPairArrayPtr,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}
/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void SrvMsgCan1UtilUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcUtilUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,SrvMsgCan1UtilUpload_PlaceHolder},
};

//CAN上传消息处理函数
void SrvMsgCan1UtilUpload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcUtilUnitArray);indexUtil++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcUtilUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcUtilUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcUtilUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcUtilUnitArray[indexUtil].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcUtilUnitArray[indexUtil].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcUtilUnitArray[indexUtil].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void SrvMsgCan1UtilUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/























