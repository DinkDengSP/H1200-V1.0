/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 23:48:15 +0800
************************************************************************************************/ 
#include "ModImplLiquidAuto.h"
#include "ModTaskHeader.h"

//任务全局对象初始化
void ModImplLiquidAutoVarInit(void)
{
    //初始化纯水计数传感器
    ModImplBaseLiquidAutoRegisterPureWaterTrigSensor();
}


//CAN1指令消息处理函数
extern void ModMsgCan1LiquidAutoCmd(void* can1CmdRecvPtr);
//CAN1上传消息处理函数
extern void ModMsgCan1LiquidAutoUpload(void* can1ReportDataPtr);
//IPC指令消息处理函数
extern void ModMsgIpcLiquidAutoCmd(void* ipcCmdRecvPtr);
//IPC上传消息处理函数
extern void ModMsgIpcLiquidAutoUpload(void* ipcReportDataPtr);

//任务接收消息处理
void ModImplLiquidAutoMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            ModMsgCan1LiquidAutoCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgCan1LiquidAutoUpload(sysMsgPtr->msgBodyPtr);
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
            ModMsgIpcLiquidAutoCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgIpcLiquidAutoUpload(sysMsgPtr->msgBodyPtr);
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
void ModImplLiquidAutoFSM_Init(void)
{
    //液路自动化状态机初始化
    ModImplBaseLiquidAutoFSM_Init();
}

//任务状态机周期运行
void ModImplLiquidAutoFSM_RunPeriod(void)
{
    //液路自动化状态机周期执行
    ModImplBaseLiquidAutoFSM_RunPeriod();
}

