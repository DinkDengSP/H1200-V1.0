/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 13:17:45 +0800
************************************************************************************************/ 
#include "CommonSrvTaskHeader.h"
#include "SrvImplSysLog.h"
#include "SrvImplBaseSysLog.h"

//任务全局对象初始化
void SrvImplSysLogVarInit(void)
{
    SrvImplBaseSysLogVarInit();
}

//CAN1指令消息处理函数
extern void SrvMsgCan1SysLogCmd(void* canMsgBodyPtr);
//CAN1上传消息处理函数
extern void SrvMsgCan1SysLogUpload(void* canMsgBodyPtr);
//CAN2指令消息处理函数
extern void SrvMsgCan2SysLogCmd(void* canMsgBodyPtr);
//CAN2上传消息处理函数
extern void SrvMsgCan2SysLogUpload(void* canMsgBodyPtr);
//IPC指令消息处理函数
extern void SrvMsgIpcSysLogCmd(void* ipcMsgBodyPtr);
//IPC上传消息处理函数
extern void SrvMsgIpcSysLogUpload(void* ipcMsgBodyPtr);
//日志消息处理函数
static void SrvMsgLogSysLogProc(void* logMsgBodyPtr);

//任务接收消息处理
void SrvImplSysLogMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            SrvMsgCan1SysLogCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan1SysLogUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg CanSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN2_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            SrvMsgCan2SysLogCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg CanSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN2_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            SrvMsgCan2SysLogUpload(sysMsgPtr->msgBodyPtr);
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
            SrvMsgIpcSysLogCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgIpcSysLogUpload(sysMsgPtr->msgBodyPtr);
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
            //调用日志消息处理函数
            SrvMsgLogSysLogProc(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Log,BodyPtr NULL!!\r\n",__func__);
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



//日志消息处理函数
static void SrvMsgLogSysLogProc(void* logMsgBodyPtr)
{
    SYS_LOG_MSG* sysLogMsgPtr = (SYS_LOG_MSG*)logMsgBodyPtr;
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
    //RTT日志输出
#if(SYSTEM_LOG_OUT_RTT_ENABLE == CONFIG_FUNC_ENABLE)
    MCU_RTT_SendBuffer(sysLogMsgPtr->logDataBuffer,sysLogMsgPtr->logDataLength);
#endif
    //UART1日志输出
#if(SYSTEM_LOG_OUT_UART1_ENABLE == CONFIG_FUNC_ENABLE)
    MCU_UART1_SendBuffer(sysLogMsgPtr->logDataBuffer,sysLogMsgPtr->logDataLength);
#endif 
    //UART3日志输出
#if(SYSTEM_LOG_OUT_UART3_ENABLE == CONFIG_FUNC_ENABLE)
    MCU_UART3_SendBuffer(sysLogMsgPtr->logDataBuffer,sysLogMsgPtr->logDataLength);
#endif 
    //CAN1 SOCKET输出
#if(CAN1_SOCKET_COMM_ENABLE == CONFIG_FUNC_ENABLE)
#if(SYSTEM_LOG_OUT_SOCKET_CAN1_ENABLE == CONFIG_FUNC_ENABLE)
    if(SET == Can1ProtInitQuery())
    {
        Can1ProtSendLogSample(sysLogMsgPtr->logDataBuffer,sysLogMsgPtr->logDataLength);
    }
#endif 
#endif
    //CAN2 SOCKET输出
#if(CAN2_SOCKET_COMM_ENABLE == CONFIG_FUNC_ENABLE)
#if(SYSTEM_LOG_OUT_SOCKET_CAN2_ENABLE == CONFIG_FUNC_ENABLE)
    if(SET == Can2ProtInitQuery())
    {
        Can2ProtSendLogSample(sysLogMsgPtr->logDataBuffer,sysLogMsgPtr->logDataLength);
    }
#endif 
#endif
    //日志处理完成,释放内存
    UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr->logDataBuffer);
    sysLogMsgPtr->logDataBuffer = NULL;
    UserMemFree(MEM_REGION_SYS_MSG_LOG,sysLogMsgPtr);
}

//任务状态机初始化
void SrvImplSysLogFSM_Init(void)
{

}

//任务状态机周期运行
void SrvImplSysLogFSM_RunPeriod(void)
{

}









