/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-29 11:46:48 +0800
************************************************************************************************/ 
#include "CommonSrvTaskHeader.h"
#include "SrvImplSysLog.h"
#include "tx_mutex.h"
#include "tx_queue.h"

//系统日志打印的互斥信号量
TX_MUTEX mutexSrvTaskSysLog;
//用于printf的缓冲区
uint8_t blockBufferSystemLog[BLOCK_SIZE_SYSTEM_LOG] = {0};
//获取锁
void SysLogGetLock(void)
{
    tx_mutex_get(&mutexSrvTaskSysLog,TX_WAIT_FOREVER);
}
//释放锁
void SysLogReleaseLock(void)
{
    tx_mutex_put(&mutexSrvTaskSysLog);
}

//任务全局对象初始化
void SrvImplSysLogVarInit(void)
{
    //创建互斥信号量
    tx_mutex_create(&mutexSrvTaskSysLog,"mutexSrvTaskSysLog",TX_INHERIT);
    //清空日志缓存长度
    UserMemClear((void*)blockBufferSystemLog,BLOCK_SIZE_SYSTEM_LOG);
}

//CAN1指令消息处理函数
extern void SrvMsgCan1SysLogCmd(void* can1MsgBodyPtr);
//CAN1上传消息处理函数
extern void SrvMsgCan1SysLogUpload(void* can1MsgBodyPtr);
//IPC指令消息处理函数
extern void SrvMsgIpcSysLogCmd(void* ipcMsgBodyPtr);
//IPC上传消息处理函数
extern void SrvMsgIpcSysLogUpload(void* ipcMsgBodyPtr);
//日志消息处理函数
static void SrvImplLogMsgProcessLogMsg(void* logMsgBodyPtr);

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
            SrvImplLogMsgProcessLogMsg(sysMsgPtr->msgBodyPtr);
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
static void SrvImplLogMsgProcessLogMsg(void* logMsgBodyPtr)
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
#if(SYSTEM_LOG_OUT_SOCKET_CAN1_ENABLE == CONFIG_FUNC_ENABLE)
    if(SET == Can1ProtInitQuery())
    {
        Can1ProtSendLogSample(sysLogMsgPtr->logDataBuffer,sysLogMsgPtr->logDataLength);
    }
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
















