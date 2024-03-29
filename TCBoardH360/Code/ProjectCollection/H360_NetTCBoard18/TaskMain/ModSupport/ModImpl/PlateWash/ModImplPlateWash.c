/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:56:33 +0800
************************************************************************************************/ 
#include "ModImplPlateWash.h"
#include "ModImplBasePlateWash.h"
#include "ModTaskHeader.h"

//任务全局对象初始化
void ModImplPlateWashVarInit(void)
{
    
}

//CAN指令消息处理函数
extern void ModMsgCan1PlateWashCmd(void* canMsgBodyPtr);
//CAN上传消息处理函数
extern void ModMsgCan1PlateWashUpload(void* canMsgBodyPtr);
//IPC指令消息处理函数
extern void ModMsgIpcPlateWashCmd(void* ipcMsgBodyPtr);
//IPC上传消息处理函数
extern void ModMsgIpcPlateWashUpload(void* ipcMsgBodyPtr);

//任务接收消息处理
void ModImplPlateWashMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            ModMsgCan1PlateWashCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgCan1PlateWashUpload(sysMsgPtr->msgBodyPtr);
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
            ModMsgIpcPlateWashCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgIpcPlateWashUpload(sysMsgPtr->msgBodyPtr);
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
void ModImplPlateWashFSM_Init(void)
{
    ModImplBasePlateWashClearTempControlCache();
    ModImplBasePlateWashTempControlStateInit();
}

//任务状态机周期运行
void ModImplPlateWashFSM_RunPeriod(void)
{
    //清洗盘控温状态机
    ModImplBasePlateWashTempControlStateRun();
    //清洗盘温度上报
    ModImplBasePlateWashTempControlReport();
}



