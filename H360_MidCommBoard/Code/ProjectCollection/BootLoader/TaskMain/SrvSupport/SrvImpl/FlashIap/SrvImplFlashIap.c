/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-31 14:37:25
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplFlashIap.h"
#include "SrvTaskHeader.h"

//SERVICE升级
extern void SrvImplBaseFlashIapServiceUpdata(BOARD_IAP_RECODER* iapRecordPtr);

//任务全局对象初始化
void SrvImplFlashIapVarInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    BOARD_IAP_RECODER iapRecord;
    //测试蜂鸣器
    BoardBeepSetState(PIN_STATE_VALID);
    CoreDelayMs(200);
    BoardBeepSetState(PIN_STATE_INVALID);
    //读取SRV升级记录
    errorSub = BoardIapReadServiceRecord(&iapRecord);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIapReadServiceRecord Failed,Code: 0X%08X\r\n",errorSub);
        return;
    }
    if(iapRecord.iapFileExistFlag == BOARD_IAP_RECODER_FLAG_EXIST)
    {
        
        //开始升级
        SystemPrintf("%s Start SrvImplBaseFlashIapServiceUpdata.\r\n",__func__);
        SrvImplBaseFlashIapServiceUpdata(&iapRecord);
    }
}

//网口指令消息处理函数
extern void SrvMsgNet8700FlashIapCmd(void* netMsgBodyPtr);
//网口上传消息处理函数
extern void SrvMsgNet8700FlashIapUpload(void* netMsgBodyPtr);

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
            SystemPrintf("E: %s Recv SysMsg Net8700Src Cmd,BodyPtr NULL!!\r\n",__func__);
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
            SystemPrintf("E: %s Recv SysMsg Net8700Src Upload,BodyPtr NULL!!\r\n",__func__);
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




