/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-16 16:18:37 +0800
************************************************************************************************/ 
#include "ModImplPlateReagent.h"
#include "ModImplBasePlateReagent.h"
#include "ModTaskHeader.h"

//任务全局对象初始化
void ModImplPlateReagentVarInit(void)
{
    
}

//CAN指令消息处理函数
extern void ModMsgCan1PlateReagentCmd(void* canMsgBodyPtr);
//CAN上传消息处理函数
extern void ModMsgCan1PlateReagentUpload(void* canMsgBodyPtr);
//IPC指令消息处理函数
extern void ModMsgIpcPlateReagentCmd(void* ipcMsgBodyPtr);
//IPC上传消息处理函数
extern void ModMsgIpcPlateReagentUpload(void* ipcMsgBodyPtr);

//任务接收消息处理
void ModImplPlateReagentMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            ModMsgCan1PlateReagentCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgCan1PlateReagentUpload(sysMsgPtr->msgBodyPtr);
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
            ModMsgIpcPlateReagentCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgIpcPlateReagentUpload(sysMsgPtr->msgBodyPtr);
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
void ModImplPlateReagentFSM_Init(void)
{
    ModImplBasePlateReagentClearTempControlCache();
    ModImplBasePlateReagentTempControlStateInit();
}

//任务状态机周期运行
void ModImplPlateReagentFSM_RunPeriod(void)
{
    //试剂盘控温状态机
    ModImplBasePlateReagentTempControlMachine();
    //试剂盘温度上报
    ModImplBasePlateReagentTempControlReport();
}



