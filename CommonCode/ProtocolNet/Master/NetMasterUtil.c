/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 10:08:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-19 11:40:34 +0800
************************************************************************************************/ 
#include "NetMasterUtil.h"

//回传指令结果
void NetMasterSendResultByRecv(NET_PROT_CMD_PACK_RECV* netCmdRecvPtr,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    NetProtSendResult(netCmdRecvPtr->srcModuleNo,netCmdRecvPtr->srcBoardID,netCmdRecvPtr->cmdCode,
                        resultPackPtr->resultCode,resultPackPtr->errorMain,resultPackPtr->errorLevel,
                        resultPackPtr->errorSub,resultParamBuffer,resultBufferLen,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //错误日志打印
        NET_PROT_LOG("%s,%d,Result: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,__LINE__,resultPackPtr->resultCode,resultPackPtr->errorMain,
                        resultPackPtr->errorLevel,resultPackPtr->errorSub);
    }
}




