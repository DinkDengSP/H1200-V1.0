/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 13:01:54 +0800
************************************************************************************************/ 
#include "CommonSrvTaskHeader.h"
#include "SrvImplUtil.h"
#include "SrvImplBaseUtil.h"

//任务全局对象初始化
void SrvImplUtilVarInit(void)
{
    SrvImplSysProbeVarInit();
}

//CAN1指令消息处理函数
extern void SrvMsgCan1UtilCmd(void* canMsgBodyPtr);
//CAN1上传消息处理函数
extern void SrvMsgCan1UtilUpload(void* canMsgBodyPtr);
//IPC指令消息处理函数
extern void SrvMsgIpcUtilCmd(void* ipcMsgBodyPtr);
//IPC上传消息处理函数
extern void SrvMsgIpcUtilUpload(void* ipcMsgBodyPtr);
//探针消息处理函数
static void SrvMsgProbeUtilMsg(void* probeMsgBodyPtr);

//任务接收消息处理
void SrvImplUtilMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            SrvMsgCan1UtilCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg CanSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            SrvMsgCan1UtilUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg CanSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC消息处理函数
            SrvMsgIpcUtilCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg IpcSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC消息处理函数
            SrvMsgIpcUtilUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg IpcSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_LOG)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用探针消息处理函数
            SrvMsgProbeUtilMsg(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg ProbeSrc,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else
    {
        //不支持的消息源,报错
        SystemPrintf("E: %s Recv SysMsg Unsupport,Src: %d\r\n",__func__,sysMsgPtr->msgSrc);
    }
    //释放消息体
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
}


//探针消息处理函数
static void SrvMsgProbeUtilMsg(void* probeMsgBodyPtr)
{
    SYS_LOG_MSG* sysLogMsgPtr = (SYS_LOG_MSG*)probeMsgBodyPtr;
    uint32_t sendResult = 0;
    if((sysLogMsgPtr->logDataBuffer == NULL)||(sysLogMsgPtr->logDataLength == 0))
    {
        if(sysLogMsgPtr->logDataBuffer != NULL)
        {
            UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataBuffer);
            sysLogMsgPtr->logDataBuffer = NULL;
        }
        UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
        return;
    }
    //探针数据发送
    SrvImplBaseUtilSendProbeBuf(sysLogMsgPtr->logDataBuffer,sysLogMsgPtr->logDataLength);
    //日志处理完成,释放内存
    UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataBuffer);
    sysLogMsgPtr->logDataBuffer = NULL;
    UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
}

//任务状态机初始化
void SrvImplUtilFSM_Init(void)
{

}

//任务状态机周期运行
void SrvImplUtilFSM_RunPeriod(void)
{
    
}














