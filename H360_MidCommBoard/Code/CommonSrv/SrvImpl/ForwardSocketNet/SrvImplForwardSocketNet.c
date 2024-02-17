/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplForwardSocketNet.h"
#include "CommonSrvTaskHeader.h"

//任务全局对象初始化
void SrvImplForwardSocketNetVarInit(void)
{

}

//任务接收消息处理
void SrvImplForwardSocketNetMsgProcess(void* taskMsgPtr)
{
    NET_PROT_FWD_DAT* forwardDatPtr = (NET_PROT_FWD_DAT*)taskMsgPtr;
    if(forwardDatPtr->forwardType == NET_PROT_FWD_TYPE_ACK_CMD)
    {
        //调用转发函数处理
        SrvImplForwardSocketNetMsgAckCmd((NET_PROT_ACK_CMD_PACK_RECV*)forwardDatPtr->forwardPackPtr);
        //释放内存,目前这个数据结构没有扩展内存,不用释放
    }
    else if(forwardDatPtr->forwardType == NET_PROT_FWD_TYPE_ACK_RESULT)
    {
        //调用转发函数处理
        SrvImplForwardSocketNetMsgAckResult((NET_PROT_ACK_RESULT_PACK_RECV*)forwardDatPtr->forwardPackPtr);
        //释放内存,目前这个数据结构没有扩展内存,不用释放
    }
    else if(forwardDatPtr->forwardType == NET_PROT_FWD_TYPE_ACK_UPLOAD)
    {
        //调用转发函数处理
        SrvImplForwardSocketNetMsgAckUpload((NET_PROT_ACK_UPLOAD_PACK_RECV*)forwardDatPtr->forwardPackPtr);
        //释放内存,目前这个数据结构没有扩展内存,不用释放
    }
    else if(forwardDatPtr->forwardType == NET_PROT_FWD_TYPE_LOG)
    {
        //调用转发函数处理
        SrvImplForwardSocketNetMsgLogMsg((NET_PROT_LOG_PACK_RECV*)forwardDatPtr->forwardPackPtr);
        //释放内存
        NET_PROT_LOG_PACK_RECV* logPackPtr = (NET_PROT_LOG_PACK_RECV*)forwardDatPtr->forwardPackPtr;
        if((logPackPtr->logBodyBufferPtr != NULL)&&(logPackPtr->logBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,logPackPtr->logBodyBufferPtr);
        }
    }
    else if(forwardDatPtr->forwardType == NET_PROT_FWD_TYPE_CMD)
    {
        //调用转发函数处理
        SrvImplForwardSocketNetMsgCmdMsg((NET_PROT_CMD_PACK_RECV*)forwardDatPtr->forwardPackPtr);
        //释放内存
        NET_PROT_CMD_PACK_RECV* cmdPackPtr = (NET_PROT_CMD_PACK_RECV*)forwardDatPtr->forwardPackPtr;
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
        }
    }
    else if(forwardDatPtr->forwardType == NET_PROT_FWD_TYPE_RESULT)
    {
        //调用转发函数处理
        SrvImplForwardSocketNetMsgResultMsg((NET_PROT_RESULT_PACK_RECV*)forwardDatPtr->forwardPackPtr);
        //释放内存
        NET_PROT_RESULT_PACK_RECV* resultPackPtr = (NET_PROT_RESULT_PACK_RECV*)forwardDatPtr->forwardPackPtr;
        if((resultPackPtr->resultParamBufferPtr != NULL)&&(resultPackPtr->resultParamBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,resultPackPtr->resultParamBufferPtr);
        }
    }
    else if(forwardDatPtr->forwardType == NET_PROT_FWD_TYPE_UPLOAD)
    {
        //调用转发函数处理
        SrvImplForwardSocketNetMsgUploadMsg((NET_PROT_UPLOAD_PACK_RECV*)forwardDatPtr->forwardPackPtr);
        //释放内存
        NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr = (NET_PROT_UPLOAD_PACK_RECV*)forwardDatPtr->forwardPackPtr;
        if((uploadPackPtr->paramBufferPtr != NULL)&&(uploadPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,uploadPackPtr->paramBufferPtr);
        }
    }
    else
    {
        //不支持的转发数据类型
        SystemPrintf("%s,Invalid Net Forward Type,Type: %d\r\n",forwardDatPtr->forwardType);
    }
    //释放主内存
    if(forwardDatPtr->forwardPackPtr != NULL)
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,forwardDatPtr->forwardPackPtr);
    }
    //释放内存
    UserMemFree(MEM_REGION_SYS_MSG_NET,forwardDatPtr);
}


//任务状态机初始化
void SrvImplForwardSocketNetFSM_Init(void)
{
    
}


//任务状态机周期运行
void SrvImplForwardSocketNetFSM_RunPeriod(void)
{
    
}







