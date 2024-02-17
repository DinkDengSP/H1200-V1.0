/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-19 00:02:08 +0800
************************************************************************************************/ 
#include "ModImplUtil.h"
#include "ModTaskHeader.h"

//任务全局对象初始化
void ModImplUtilVarInit(void)
{
    
}

//CAN指令消息处理函数
extern void ModMsgCan1UtilCmd(void* canMsgBodyPtr);
//CAN上传消息处理函数
extern void ModMsgCan1UtilUpload(void* canMsgBodyPtr);
//IPC指令消息处理函数
extern void ModMsgIpcUtilCmd(void* ipcMsgBodyPtr);
//IPC上传消息处理函数
extern void ModMsgIpcUtilUpload(void* ipcMsgBodyPtr);

//任务接收消息处理
void ModImplUtilMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            ModMsgCan1UtilCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgCan1UtilUpload(sysMsgPtr->msgBodyPtr);
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
            ModMsgIpcUtilCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgIpcUtilUpload(sysMsgPtr->msgBodyPtr);
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
void ModImplUtilFSM_Init(void)
{
    //灯光显示延迟
    CoreDelayMs(1000);
    //初始化状态机
    ModImplBaseUtilInitTempControlMachine();
    //初始化错误状态机
    ModImplBaseUtilInitErrorShowMachine();
}

//任务状态机周期运行
void ModImplUtilFSM_RunPeriod(void)
{
    //环境温度控温状态LED
    ModImplBaseUtilEnvirLedTempControlMachine();
    //反应盘控温状态LED
    ModImplBaseUtilPlateReactionLedTempControlMachine();
    //清洗盘控温状态LED
    ModImplBaseUtilPlateWashLedTempControlMachine();
    //试剂盘控温状态LED
    ModImplBaseUtilPlateReagentLedTempControlMachine();
    //板上温度通道错误状态LED
    ModImplBaseUtilBoardLedErrorShowMachine();
    //环境温度通道错误状态LED
    ModImplBaseUtilEnvirLedErrorShowMachine();
    //反应盘通道错误状态LED
    ModImplBaseUtilPlateReactionLedErrorShowMachine();
    //清洗盘通道错误状态LED
    ModImplBaseUtilPlateWashLedErrorShowMachine();
    //试剂盘通道错误状态LED
    ModImplBaseUtilPlateReagentLedErrorShowMachine();
}














