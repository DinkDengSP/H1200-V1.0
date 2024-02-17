/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 08:40:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-23 14:08:39 +0800
************************************************************************************************/ 
#include "CanProtPort.h"
#include "CanProtBase.h"
#include "DriverConfigSystem.h"
#include "UserMemManage.h"
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"

/*******************************************转发接口*****************************************/
//发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
//CAN1_PROT 指令信息转发
void Can1ProtCmdMsgForward(CAN_PROT_CMD_PACK* cmdPackPtr)
{
    SystemPrintf("%s ,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Not Support\r\n",__func__,
                        cmdPackPtr->srcModuleNo,cmdPackPtr->srcBoardID,cmdPackPtr->targetModuleNo,
                        cmdPackPtr->targetBoardID);
    return;
}

//CAN1_PROT AckCmd数据转发
ERROR_SUB Can1ProtAckCmdForward(CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr)
{
    SystemPrintf("%s ,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Not Support\r\n",__func__,
                        ackCmdPackPtr->srcModuleNo,ackCmdPackPtr->srcBoardID,ackCmdPackPtr->targetModuleNo,
                        ackCmdPackPtr->targetBoardID);
    return ERROR_SUB_OK;
}

//CAN1_PROT 结果信息转发
void Can1ProtResultMsgForward(CAN_PROT_RESULT_PACK* resultPackPtr)
{
    SystemPrintf("%s ,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Not Support\r\n",__func__,
                        resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->targetModuleNo,
                        resultPackPtr->targetBoardID);
    return;
}

//CAN1_PROT AckResult数据转发
ERROR_SUB Can1ProtAckResultForward(CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr)
{
    SystemPrintf("%s ,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Not Support\r\n",__func__,
                        ackResultPackPtr->srcModuleNo,ackResultPackPtr->srcBoardID,ackResultPackPtr->targetModuleNo,
                        ackResultPackPtr->targetBoardID);
    return ERROR_SUB_OK;
}

//CAN1_PROT 主动上传信息抓发
void Can1ProtUploadMsgForward(CAN_PROT_UPLOAD_PACK* uploadPackPtr)
{
    SystemPrintf("%s ,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Not Support\r\n",__func__,
                        uploadPackPtr->srcModuleNo,uploadPackPtr->srcBoardID,uploadPackPtr->targetModuleNo,
                        uploadPackPtr->targetBoardID);
    return;
}

//CAN1_PROT AckUpload数据转发
ERROR_SUB Can1ProtAckUploadForward(CAN_PROT_ACK_UPLOAD_PACK* ackUploadPackPtr)
{
    SystemPrintf("%s ,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Not Support\r\n",__func__,
                        ackUploadPackPtr->srcModuleNo,ackUploadPackPtr->srcBoardID,ackUploadPackPtr->targetModuleNo,
                        ackUploadPackPtr->targetBoardID);
    return ERROR_SUB_OK;
}

//CAN1_PROT 日志信息转发
void Can1ProtLogMsgForward(CAN_PROT_LOG_PACK* logPackPtr)
{
    SystemPrintf("%s ,SrcMod: %d,SrcBoard: %d,TargetMod: %d,TargetBoard: %d,Not Support\r\n",__func__,
                        logPackPtr->srcModuleNo,logPackPtr->srcBoardID,logPackPtr->targetModuleNo,
                        logPackPtr->targetBoardID);
    return;
}









