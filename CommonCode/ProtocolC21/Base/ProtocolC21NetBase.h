/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 13:23:34 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_C21_NET_BASE_H_
#define __PROTOCOL_C21_NET_BASE_H_
#include "ProtocolC21NetUtil.h"

//初始化SOCKET
void ProtocolC21NetSocketInit(void);

//检查SOCKET是否已经初始化
FlagStatus ProtocolC21NetSocketInitQuery(void);

//SOCKET数据包接收
void ProtocolC21NetSocketPackRecv(NET_RECV_PACK* netRecvPackPtr,uint32_t currentTickMs);

//SOCKET内部周期性数据处理
void ProtocolC21NetRunPeriod(uint32_t currentTickMs);

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB ProtocolC21NetSendCmdOnly(uint16_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength);

//SOCKET发送指令ACK包
ERROR_SUB ProtocolC21NetSendAckCmd(C21_ACK_RESULT ackResult);


#endif





