/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:56:33 +0800
************************************************************************************************/ 
#include "SrvImplTempSample.h"
#include "SrvImplBaseTempSample.h"
#include "CommonSrvTaskHeader.h"

//任务全局对象初始化
void SrvImplTempSampleVarInit(void)
{
    
}

//CAN指令消息处理函数
extern void SrvMsgCan1TempSampleCmd(void* canMsgBodyPtr);
//CAN上传消息处理函数
extern void SrvMsgCan1TempSampleUpload(void* canMsgBodyPtr);
//IPC指令消息处理函数
extern void SrvMsgIpcTempSampleCmd(void* ipcMsgBodyPtr);
//IPC上传消息处理函数
extern void SrvMsgIpcTempSampleUpload(void* ipcMsgBodyPtr);

//任务接收消息处理
void SrvImplTempSampleMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            SrvMsgCan1TempSampleCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan1TempSampleUpload(sysMsgPtr->msgBodyPtr);
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
            SrvMsgIpcTempSampleCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgIpcTempSampleUpload(sysMsgPtr->msgBodyPtr);
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
void SrvImplTempSampleFSM_Init(void)
{
    //板上温度传感器状态机初始化
    SrvImplBaseTempSample_FSM_InitBoardSensor();
    //环境温度传感器状态机初始化
    SrvImplBaseTempSample_FSM_InitEnvirSensor();
    //反应盘温度传感器状态机初始化
    SrvImplBaseTempSample_FSM_InitPlateReactionSensor();
    //清洗盘温度传感器状态机初始化
    SrvImplBaseTempSample_FSM_InitPlateWashSensor();
    //试剂盘温度传感器状态机初始化
    SrvImplBaseTempSample_FSM_InitPlateReagentSensor();
}

//任务状态机周期运行
void SrvImplTempSampleFSM_RunPeriod(void)
{
    //板上温度传感器状态机周期运转
    SrvImplBaseTempSample_FSM_PeriodBoardSensor();
    //环境温度传感器状态机周期运转
    SrvImplBaseTempSample_FSM_PeriodEnvirSensor();
    //反应盘温度传感器状态机周期运转
    SrvImplBaseTempSample_FSM_PeriodPlateReactionSensor();
    //清洗盘温度传感器状态机周期运转
    SrvImplBaseTempSample_FSM_PeriodPlateWashSensor();
    //试剂盘温度传感器状态机周期运转
    SrvImplBaseTempSample_FSM_PeriodPlateReagentSensor();
}





