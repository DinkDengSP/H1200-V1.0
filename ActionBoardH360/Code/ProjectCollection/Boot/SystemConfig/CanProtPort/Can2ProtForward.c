/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 08:40:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-23 14:09:26 +0800
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
//CAN2_PROT 指令信息转发
void Can2ProtCmdMsgForward(CAN_PROT_CMD_PACK* cmdPackPtr)
{
    asm("nop");
}

//CAN2_PROT AckCmd数据转发
ERROR_SUB Can2ProtAckCmdForward(CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr)
{
    return ERROR_SUB_OK;
}

//CAN2_PROT 结果信息转发
void Can2ProtResultMsgForward(CAN_PROT_RESULT_PACK* resultPackPtr)
{
    asm("nop");
}

//CAN2_PROT AckResult数据转发
ERROR_SUB Can2ProtAckResultForward(CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr)
{
    return ERROR_SUB_OK;
}

//CAN2_PROT 主动上传信息抓发
void Can2ProtUploadMsgForward(CAN_PROT_UPLOAD_PACK* uploadPackPtr)
{
    asm("nop");
}

//CAN2_PROT AckUpload数据转发
ERROR_SUB Can2ProtAckUploadForward(CAN_PROT_ACK_UPLOAD_PACK* ackUploadPackPtr)
{
    return ERROR_SUB_OK;
}

//CAN2_PROT 日志信息转发
void Can2ProtLogMsgForward(CAN_PROT_LOG_PACK* logPackPtr)
{
    asm("nop");
}













