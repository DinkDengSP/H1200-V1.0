/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 14:38:25 +0800
************************************************************************************************/ 
#include "ModImplUtil.h"
#include "ModTaskHeader.h"

//任务全局对象初始化
void ModImplUtilVarInit(void)
{

}

//网口指令消息处理函数
extern void ModMsgNetUtilCmd(void* netCmdRecvPtr);
//网口上传消息处理函数
extern void ModMsgNetUtilUpload(void* netReportDataPtr);
//CAN1指令消息处理函数
extern void ModMsgCan1UtilCmd(void* can1CmdRecvPtr);
//CAN1上传消息处理函数
extern void ModMsgCan1UtilUpload(void* can1ReportDataPtr);

//任务接收消息处理
void ModImplUtilMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_NET_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用网口消息处理函数
            ModMsgNetUtilCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgNetUtilUpload(sysMsgPtr->msgBodyPtr);
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
            ModMsgCan1UtilCmd(sysMsgPtr->msgBodyPtr);
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
            ModMsgCan1UtilUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Upload,BodyPtr NULL!!\r\n",__func__);
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
    
}

//任务状态机周期运行
void ModImplUtilFSM_RunPeriod(void)
{
    
}














