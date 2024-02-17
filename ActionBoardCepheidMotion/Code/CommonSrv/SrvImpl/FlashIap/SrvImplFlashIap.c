/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 15:48:23 +0800
************************************************************************************************/ 
#include "CommonSrvTaskHeader.h"
#include "SrvImplFlashIap.h"

//BOOT升级
extern void SrvImplFlashIapBootUpdata(BOARD_IAP_RECODER* iapRecordPtr);

//任务全局对象初始化
void SrvImplFlashIapVarInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    BOARD_IAP_RECODER iapRecord;
    //初始化延迟,等待系统稳定
    CoreDelayMs(100);
    //读取BOOT升级记录
    errorSub = BoardIapReadBootRecord(&iapRecord);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIapReadBootRecord Failed,Code: 0X%08X\r\n",errorSub);
        return;
    }
    //BOOT程序升级
    if(iapRecord.iapFileExistFlag == BOARD_IAP_RECODER_FLAG_EXIST)
    {
        SystemPrintf("%s Start SrvImplFlashIapBootUpdata.\r\n",__func__);
        SrvImplFlashIapBootUpdata(&iapRecord);
    }
}

//CAN1指令消息处理函数
extern void SrvMsgCan1FlashIapCmd(void* canMsgBodyPtr);
//CAN1上传消息处理函数
extern void SrvMsgCan1FlashIapUpload(void* canMsgBodyPtr);
//CAN2指令消息处理函数
extern void SrvMsgCan2FlashIapCmd(void* canMsgBodyPtr);
//CAN2上传消息处理函数
extern void SrvMsgCan2FlashIapUpload(void* canMsgBodyPtr);
//IPC指令消息处理函数
extern void SrvMsgIpcFlashIapCmd(void* ipcMsgBodyPtr);
//IPC上传消息处理函数
extern void SrvMsgIpcFlashIapUpload(void* ipcMsgBodyPtr);

//任务接收消息处理
void SrvImplFlashIapMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN消息处理函数
            SrvMsgCan1FlashIapCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan1FlashIapUpload(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan2FlashIapCmd(sysMsgPtr->msgBodyPtr);
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
            SrvMsgCan2FlashIapUpload(sysMsgPtr->msgBodyPtr);
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
            //调用IPC消息处理函数
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









