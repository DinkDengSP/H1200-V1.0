/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 11:27:54 +0800
************************************************************************************************/ 
#include "ModImplMeasureModule.h"
#include "ModTaskHeader.h"

//任务全局对象初始化
void ModImplMeasureModuleVarInit(void)
{
    
}


//CAN1指令消息处理函数
extern void ModMsgCan1MeasureModuleCmd(void* can1CmdRecvPtr);
//CAN1上传消息处理函数
extern void ModMsgCan1MeasureModuleUpload(void* can1ReportDataPtr);
//IPC指令消息处理函数
extern void ModMsgIpcMeasureModuleCmd(void* ipcCmdRecvPtr);
//IPC上传消息处理函数
extern void ModMsgIpcMeasureModuleUpload(void* ipcReportDataPtr);

//任务接收消息处理
void ModImplMeasureModuleMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            ModMsgCan1MeasureModuleCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            ModMsgCan1MeasureModuleUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC消息处理函数
            ModMsgIpcMeasureModuleCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg IpcSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC消息处理函数
            ModMsgIpcMeasureModuleUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg IpcSrc Upload,BodyPtr NULL!!\r\n",__func__);
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

//任务状态机初始化
void ModImplMeasureModuleFSM_Init(void)
{
    ModImplBaseMeasureModuleFSM_Init();
}

//任务状态机周期运行
void ModImplMeasureModuleFSM_RunPeriod(void)
{
    ModImplBaseMeasureModuleFSM_RunPeriod();
}













