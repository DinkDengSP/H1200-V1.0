/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-29 16:50:02 +0800
************************************************************************************************/ 
#include "SrvImplUtil.h"
#include "SrvImplBaseUtil.h"
#include "CommonSrvTaskHeader.h"

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
//设置系统运行模式
static void SrvMsgCan1UtilCmd_SetRunningMode(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

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
    {CAN_MASTER_CMD_UTIL_SET_RUNNING_MODE,1  ,ENABLE ,SrvMsgCan1UtilCmd_SetRunningMode  },
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
    //获取联机模式
    PROT_CONNECT_FLAG protConnectFlag = (PROT_CONNECT_FLAG)(cmdPackPtr->paramBufferPtr[0]);
    //记录下主控的模块号和板号
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsg = SysStateGetMainControlMsgPtr();
    //系统状态
    mainControlMsg->moduleNo = cmdPackPtr->srcModuleNo;
    mainControlMsg->boardID = cmdPackPtr->srcBoardID;
    mainControlMsg->onlineFlag = SET;
    mainControlMsg->protConnectFlag = protConnectFlag;
    //日志显示指令
    SystemPrintf("%s,ConnectFlag: %d\r\n",__func__,(uint8_t)protConnectFlag);
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
    //记录下主控的模块号和板号
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsg = SysStateGetMainControlMsgPtr();
    //系统状态
    mainControlMsg->moduleNo = cmdPackPtr->srcModuleNo;
    mainControlMsg->boardID = cmdPackPtr->srcBoardID;
    //设置脱机
    mainControlMsg->onlineFlag = RESET;
    //脱机自动将联机模式修改为调试软件联机
    mainControlMsg->protConnectFlag = PROT_CONNECT_FLAG_DEBUG;
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

//设置系统运行模式
static void SrvMsgCan1UtilCmd_SetRunningMode(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    SYS_RUNNING_MODE sysRunningMode = (SYS_RUNNING_MODE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,sysRunningMode: %d\r\n",__func__,(uint8_t)sysRunningMode);
    //记录下主控的模块号和板号
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsg = SysStateGetMainControlMsgPtr();
    //系统状态
    mainControlMsg->sysRunningMode = sysRunningMode;
    //执行设置系统运行模式
    SrvImplBaseUtilSetRunningMode(sysRunningMode,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void SrvMsgCan1UtilUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcUnitArray[] = 
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
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcUnitArray);indexUtil++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcUnitArray[indexUtil].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcUnitArray[indexUtil].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcUnitArray[indexUtil].canUploadProcFuncPtr(canUploadPackPtr);
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









