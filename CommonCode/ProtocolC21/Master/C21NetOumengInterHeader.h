/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-06 09:37:40 +0800
************************************************************************************************/ 
#ifndef __C21NET_OUMENG_INTER_HEADER_H_
#define __C21NET_OUMENG_INTER_HEADER_H_
#include "C21NetOumengInterDataType.h"
#include "C21NetOumengInterBase.h"

/*************************************************************************************************************/
//处理NET指令的实现代码
typedef void (*UserC21NetCmdProcessFunc)(C21_NET_PACKAGE* peUartCmdMsgPtr, void* appStatus);

//网口指令处理单元
typedef struct C21_NET_CMD_PROC_UNIT
{
    MACRO_C21_NET_CMD commandCode;//指令码
    uint16_t exceptParamLengthMin;//预期参数最小长度
    UserC21NetCmdProcessFunc c21NetCmdProcFuncPtr;
}C21_NET_CMD_PROC_UNIT;


#endif









