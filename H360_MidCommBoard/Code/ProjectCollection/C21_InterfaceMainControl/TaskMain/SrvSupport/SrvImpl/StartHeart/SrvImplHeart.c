/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 14:03:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplStartHeart.h"
#include "SrvImplBaseHeart.h"
#include "SrvTaskHeader.h"


/*--------------------------系统启动状态机相关函数------------------------------*/
//任务全局对象初始化
void SrvImplStartHeartVarInit(void)
{
    //测试蜂鸣器
    BoardBeepSetState(PIN_STATE_VALID);
    CoreDelayMs(500);
    BoardBeepSetState(PIN_STATE_INVALID);
}


//网口指令消息处理函数
extern void SrvMsgNet8700HeartCmd(void* net8700CmdRecvPtr);
//网口上传消息处理函数
extern void SrvMsgNet8700HeartUpload(void* net8700ReportDataPtr);
//CAN1指令消息处理函数
extern void SrvMsgCan1HeartCmd(void* can1CmdRecvPtr);
//CAN1上传消息处理函数
extern void SrvMsgCan1HeartUpload(void* can1ReportDataPtr);
//CAN2指令消息处理函数
extern void SrvMsgCan2HeartCmd(void* can2CmdRecvPtr);
//CAN2上传消息处理函数
extern void SrvMsgCan2HeartUpload(void* can2ReportDataPtr);
//串口指令消息处理函数
extern void SrvMsgUartHeartCmd(void* uartCmdRecvPtr);
//串口上传消息处理函数
extern void SrvMsgUartHeartUpload(void* uartReportDataPtr);
//IPC指令消息处理函数
extern void SrvMsgIpcHeartCmd(void* ipcCmdRecvPtr);
//IPC上传消息处理函数
extern void SrvMsgIpcHeartUpload(void* ipcReportDataPtr);

//任务接收消息处理
void SrvImplStartHeartMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_NET_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用网口消息处理函数
            SrvMsgNet8700HeartCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg NetSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_NET_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用网口消息处理函数
            SrvMsgNet8700HeartUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg NetSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            SrvMsgCan1HeartCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            SrvMsgCan1HeartUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN2_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN2消息处理函数
            SrvMsgCan2HeartCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can2Src Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN2_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN2消息处理函数
            SrvMsgCan2HeartUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can2Src Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_UART_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用串口消息处理函数
            SrvMsgUartHeartCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg UartSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_UART_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用串口消息处理函数
            SrvMsgUartHeartUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg UartSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC指令处理函数
            SrvMsgIpcHeartCmd(sysMsgPtr->msgBodyPtr);
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
            //调用IPC上传消息处理函数
            SrvMsgIpcHeartUpload(sysMsgPtr->msgBodyPtr);
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
void SrvImplStartHeartFSM_Init(void)
{
    //系统运行状态灯显示
#if(SRV_HEART_LED_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubLedShowInit();
#endif
    //窗口看门狗初始化
#if(SRV_HEART_WWDG_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubWwdgInit();
#endif
    //系统线程运行性能统计初始化
#if(SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubThreadPerformanceStateInit();
#endif
    //系统RTC时间显示初始化
#if(SRV_HEART_RTC_TIMER_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubRtcTimerShowInit();
#endif
    //系统内存使用率展示初始化
#if(SRV_HEART_MEM_USAGE_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubMemUsageShowInit();
#endif
    //独立看门狗初始化
#if(SRV_HEART_IWDG_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubIwdgInit();
#endif
}

//任务状态机周期运行
void SrvImplStartHeartFSM_RunPeriod(void)
{
    //系统运行状态灯显示
#if(SRV_HEART_LED_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
    //LED定时闪烁
    SrvImplHeartSubLedShowRunPeriod();
#endif
    //窗口看门狗周期喂狗
#if(SRV_HEART_WWDG_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubWwdgFeedRunPeriod();
#endif
    //系统线程运行性能统计周期运行
#if(SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubThreadPerformanceStateRunPeriod();
#endif 
    //系统RTC时间显示周期运行
#if(SRV_HEART_RTC_TIMER_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubRtcTimerShowRunPeriod();
#endif
    //系统内存使用率展示周期运行
#if(SRV_HEART_MEM_USAGE_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubMemUsageShowRunPeriod();
#endif
    //独立看门狗周期运行喂狗
#if(SRV_HEART_IWDG_ENABLE != CONFIG_FUNC_DISABLE)
    SrvImplHeartSubIwdgFeedRunPeriod();
#endif
}




