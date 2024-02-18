/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 15:02:56 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_HEADER_H_
#define __CAN_MASTER_HEADER_H_
#include "CanMasterUtil.h"
#include "CanMasterReport.h"
#include "CanMasterFlashIap.h"
#include "CanMasterParam.h"
#include "CanMasterPortIn.h"
#include "CanMasterPortOut.h"
#include "CanMasterUtil.h"
#include "CanMasterStepMotor.h"

/*************************************************************************************************************/
//处理CAN指令的实现代码
typedef void (*UserCanCmdProFunc)(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理单元
typedef struct USER_CAN_CMD_PROC_UNIT
{
    uint32_t commandCode;//指令码
    uint32_t exceptParamLengthMin;//预期参数最小长度
    FunctionalState needResultReply;//是否需要结果回复
    UserCanCmdProFunc canCmdProcFuncPtr;
}USER_CAN_CMD_PROC_UNIT;

/*************************************************************************************************************/
//处理CAN上报消息的实现代码
typedef void (*UserCanUploadProcessFunc)(CAN_PROT_UPLOAD_PACK* uploadPackPtr);
//CAN上报消息处理单元
typedef struct USER_CAN_UPLOAD_PROC_UNIT
{
    uint32_t uploadId;//上报ID
    uint32_t exceptParamLengthMin;//预期参数最小长度
    UserCanUploadProcessFunc canUploadProcFuncPtr;//上报ID处理函数
}USER_CAN_UPLOAD_PROC_UNIT;

#endif

































