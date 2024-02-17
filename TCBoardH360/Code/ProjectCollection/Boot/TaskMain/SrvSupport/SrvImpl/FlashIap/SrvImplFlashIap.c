/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 13:01:27 +0800
************************************************************************************************/ 
#include "SrvImplFlashIap.h"
#include "SrvTaskHeader.h"

//导入外部程序
//Srv升级
extern void SrvImplFlashIapSrvUpdata(BOARD_IAP_RECODER* iapRecordPtr);
//App升级
extern void SrvImplFlashIapAppUpdata(BOARD_IAP_RECODER* iapRecordPtr);

//任务全局对象初始化
void SrvImplFlashIapVarInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    BOARD_IAP_RECODER iapRecord;
    //初始化蜂鸣器动作
    BoardBeepSetState(PIN_STATE_VALID);
    //初始化延迟,等待系统稳定
    CoreDelayMs(100);
    BoardBeepSetState(PIN_STATE_INVALID);
    //读取Srv升级记录
    errorSub = BoardIapReadSrvRecord(&iapRecord);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIapReadSrvRecord Failed,Code: 0X%08X\r\n",errorSub);
        return;
    }
    //Srv程序升级
    if(iapRecord.iapFileExistFlag == BOARD_IAP_RECODER_FLAG_EXIST)
    {
        SystemPrintf("%s Start SrvImplFlashIapSrvUpdata.\r\n",__func__);
        SrvImplFlashIapSrvUpdata(&iapRecord);
    }
    //读取App升级记录
    errorSub = BoardIapReadAppRecord(&iapRecord);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIapReadAppRecord Failed,Code: 0X%08X\r\n",errorSub);
        return;
    }
    //App程序升级
    if(iapRecord.iapFileExistFlag == BOARD_IAP_RECODER_FLAG_EXIST)
    {
        SystemPrintf("%s Start SrvImplFlashIapAppUpdata.\r\n",__func__);
        SrvImplFlashIapAppUpdata(&iapRecord);
    }
}

//CAN指令消息处理函数
extern void SrvMsgCan1FlashIapCmd(void* canMsgBodyPtr);
//CAN上传消息处理函数
extern void SrvMsgCan1FlashIapUpload(void* canMsgBodyPtr);

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











