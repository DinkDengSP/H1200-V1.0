/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 10:08:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-19 11:43:08 +0800
************************************************************************************************/ 
#ifndef __UART_MASTER_UTIL_H_
#define __UART_MASTER_UTIL_H_
#include "UartMasterDataType.h"

//回传指令结果
void UartMasterSendResultByRecv(UART_PROT_CMD_PACK_RECV* uartCmdRecvPtr,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen);

#endif


