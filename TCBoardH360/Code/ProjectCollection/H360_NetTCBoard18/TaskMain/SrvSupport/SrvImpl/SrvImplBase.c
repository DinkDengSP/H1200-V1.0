/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 11:49:02 +0800
************************************************************************************************/ 
#include "SrvImplBase.h"

//探针数据发送实现
void BoardProbeSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength)
{
    MCU_RTT_SendBuffer(bufferPtr,bufferLength);
}
























