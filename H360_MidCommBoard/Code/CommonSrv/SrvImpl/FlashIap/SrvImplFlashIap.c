/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 14:04:03
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplFlashIap.h"
#include "SrvImplBaseFlashIap.h"

//任务全局对象初始化
void SrvImplFlashIapVarInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    BOARD_IAP_RECODER iapRecord;
    //初始化延迟,等待系统稳定
    CoreDelayMs(100);
    //读取Boot升级记录
    errorSub = BoardIapReadBootRecord(&iapRecord);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIapReadBootRecord Failed,Code: 0X%08X\r\n",errorSub);
        return;
    }
    if(iapRecord.iapFileExistFlag == BOARD_IAP_RECODER_FLAG_EXIST)
    {
        SystemPrintf("%s Start SrvImplBaseFlashIapBootUpdata.\r\n",__func__);
        SrvImplBaseFlashIapBootUpdata(&iapRecord);
    }
}


//网口指令消息处理函数
extern void SrvMsgNet8700FlashIapCmd(void* net8700CmdRecvPtr);
//网口上传消息处理函数
extern void SrvMsgNet8700FlashIapUpload(void* net8700ReportDataPtr);
//CAN1指令消息处理函数
extern void SrvMsgCan1FlashIapCmd(void* can1CmdRecvPtr);
//CAN1上传消息处理函数
extern void SrvMsgCan1FlashIapUpload(void* can1ReportDataPtr);
//CAN2指令消息处理函数
extern void SrvMsgCan2FlashIapCmd(void* can2CmdRecvPtr);
//CAN2上传消息处理函数
extern void SrvMsgCan2FlashIapUpload(void* can2ReportDataPtr);
//串口指令消息处理函数
extern void SrvMsgUartFlashIapCmd(void* uartCmdRecvPtr);
//串口上传消息处理函数
extern void SrvMsgUartFlashIapUpload(void* uartReportDataPtr);
//IPC指令消息处理函数
extern void SrvMsgIpcFlashIapCmd(void* ipcCmdRecvPtr);
//IPC上传消息处理函数
extern void SrvMsgIpcFlashIapUpload(void* ipcReportDataPtr);

//任务接收消息处理
void SrvImplFlashIapMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_NET_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用网口消息处理函数
            SrvMsgNet8700FlashIapCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgNet8700FlashIapUpload(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan1FlashIapCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan1FlashIapUpload(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan2FlashIapCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan2FlashIapUpload(sysMsgPtr->msgBodyPtr);
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
            SrvMsgUartFlashIapCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgUartFlashIapUpload(sysMsgPtr->msgBodyPtr);
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
            SrvMsgIpcFlashIapCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgIpcFlashIapUpload(sysMsgPtr->msgBodyPtr);
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
void SrvImplFlashIapFSM_Init(void)
{
    
}


//任务状态机周期运行
void SrvImplFlashIapFSM_RunPeriod(void)
{
    
}









