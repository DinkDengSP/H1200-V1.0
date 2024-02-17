/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 15:22:09
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplForwardSocketCan.h"
#include "CommonSrvTaskHeader.h"


//任务全局对象初始化
void SrvImplForwardSocketCanVarInit(void)
{

}

//任务接收消息处理
void SrvImplForwardSocketCanMsgProcess(void* taskMsgPtr)
{
    CAN_PROT_FWD_DAT* forwardDatPtr = (CAN_PROT_FWD_DAT*)taskMsgPtr;
    if(forwardDatPtr->forwardType == CAN_PROT_FWD_TYPE_ACK_CMD)
    {
        //调用转发函数处理
        SrvImplForwardSocketCanMsgAckCmd(forwardDatPtr->portNo,(CAN_PROT_ACK_CMD_PACK*)forwardDatPtr->forwardPackPtr);
        //释放内存,目前这个数据结构没有扩展内存,不用释放
    }
    else if(forwardDatPtr->forwardType == CAN_PROT_FWD_TYPE_ACK_RESULT)
    {
        //调用转发函数处理
        SrvImplForwardSocketCanMsgAckResult(forwardDatPtr->portNo,(CAN_PROT_ACK_RESULT_PACK*)forwardDatPtr->forwardPackPtr);
        //释放内存,目前这个数据结构没有扩展内存,不用释放
    }
    else if(forwardDatPtr->forwardType == CAN_PROT_FWD_TYPE_ACK_UPLOAD)
    {
        //调用转发函数处理
        SrvImplForwardSocketCanMsgAckUpload(forwardDatPtr->portNo,(CAN_PROT_ACK_UPLOAD_PACK*)forwardDatPtr->forwardPackPtr);
        //释放内存,目前这个数据结构没有扩展内存,不用释放
    }
    else if(forwardDatPtr->forwardType == CAN_PROT_FWD_TYPE_LOG)
    {
        //调用转发函数处理
        SrvImplForwardSocketCanMsgLogMsg(forwardDatPtr->portNo,(CAN_PROT_LOG_PACK*)forwardDatPtr->forwardPackPtr);
        //释放内存
        CAN_PROT_LOG_PACK* logPackPtr = (CAN_PROT_LOG_PACK*)forwardDatPtr->forwardPackPtr;
        if((logPackPtr->logDataBufferPtr != NULL)&&(logPackPtr->logDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,logPackPtr->logDataBufferPtr);
        }
    }
    else if(forwardDatPtr->forwardType == CAN_PROT_FWD_TYPE_CMD)
    {
        //调用转发函数处理
        SrvImplForwardSocketCanMsgCmdMsg(forwardDatPtr->portNo,(CAN_PROT_CMD_PACK*)forwardDatPtr->forwardPackPtr);
        //释放内存
        CAN_PROT_CMD_PACK* cmdPackPtr = (CAN_PROT_CMD_PACK*)forwardDatPtr->forwardPackPtr;
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
    }
    else if(forwardDatPtr->forwardType == CAN_PROT_FWD_TYPE_RESULT)
    {
        //调用转发函数处理
        SrvImplForwardSocketCanMsgResultMsg(forwardDatPtr->portNo,(CAN_PROT_RESULT_PACK*)forwardDatPtr->forwardPackPtr);
        //释放内存
        CAN_PROT_RESULT_PACK* resultPackPtr = (CAN_PROT_RESULT_PACK*)forwardDatPtr->forwardPackPtr;
        if((resultPackPtr->resultParamBufferPtr != NULL)&&(resultPackPtr->resultParamBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,resultPackPtr->resultParamBufferPtr);
        }
    }
    else if(forwardDatPtr->forwardType == CAN_PROT_FWD_TYPE_UPLOAD)
    {
        //调用转发函数处理
        SrvImplForwardSocketCanMsgUploadMsg(forwardDatPtr->portNo,(CAN_PROT_UPLOAD_PACK*)forwardDatPtr->forwardPackPtr);
        //释放内存
        CAN_PROT_UPLOAD_PACK* uploadPackPtr = (CAN_PROT_UPLOAD_PACK*)forwardDatPtr->forwardPackPtr;
        if((uploadPackPtr->uploadDataBufferPtr != NULL)&&(uploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,uploadPackPtr->uploadDataBufferPtr);
        }
    }
    else
    {
        //不支持的转发数据类型
        SystemPrintf("%s,Invalid Can Forward Type,Type: %d,portCan: %d\r\n",forwardDatPtr->forwardType,
                        forwardDatPtr->portNo);
    }
    //释放主内存
    if(forwardDatPtr->forwardPackPtr != NULL)
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,forwardDatPtr->forwardPackPtr);
    }
    //释放内存
    UserMemFree(MEM_REGION_SYS_MSG_CAN,forwardDatPtr);
}


//任务状态机初始化
void SrvImplForwardSocketCanFSM_Init(void)
{
    
}


//任务状态机周期运行
void SrvImplForwardSocketCanFSM_RunPeriod(void)
{
    
}









