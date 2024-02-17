/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-05 17:20:52 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_C21_NET_MAIN_H_
#define __PROTOCOL_C21_NET_MAIN_H_
#include "ProtocolC21NetBase.h"
#include "ProtocolC21NetUtil.h"

//发送指令,使用结构体指针作为参数
ERROR_SUB ProtocolC21NetSocketSendCmdSample(C21_NET_PACKAGE* cmdSendPtr);

//发送指令,使用详细信息作为参数
ERROR_SUB ProtocolC21NetSocketSendCmdDetail(uint16_t cmdCode,uint8_t* dataBody,uint16_t dataBodyLength);

#endif








