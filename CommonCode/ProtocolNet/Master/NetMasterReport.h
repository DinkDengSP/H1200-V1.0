/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 10:08:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-19 11:40:34 +0800
************************************************************************************************/ 
#ifndef __NET_MASTER_REPORT_H_
#define __NET_MASTER_REPORT_H_
#include "NetMasterDataType.h"

//网口报错
void NetMasterReportErrorToPC(ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub);

//网口上报消息
void NetMasterReportMsgToPC(uint32_t reportCmd,uint8_t *reportBuf,uint16_t reportLength);

#endif









